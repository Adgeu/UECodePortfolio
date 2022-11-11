// For educational purposes only. 


#include "Components/Drone/DroneControllerComponent.h"

#include "Characters/Animations/Notifies/AnimNotify_LaunchDrone.h"
#include "Components/CapsuleComponent.h"
#include "Drone/DroneBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widgets/Drone/DroneWidget.h"
#include "Utils/XHAnimUtils.h"
#include "Utils/XHMathUtils.h"
#include "Utils/XHTraceUtils.h"


UDroneControllerComponent::UDroneControllerComponent()
{
	SetIsReplicatedByDefault(true);
}

void UDroneControllerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDroneControllerComponent, DronePool);
	DOREPLIFETIME(UDroneControllerComponent, ActiveDrones);
	DOREPLIFETIME(UDroneControllerComponent, CurrentDronePoolIndex);
	DOREPLIFETIME(UDroneControllerComponent, CurrentConnectedDrone);
	DOREPLIFETIME(UDroneControllerComponent, AvailableDroneCount);
}

ADroneBase* UDroneControllerComponent::GetCurrentConnectedDrone() const
{
	return CurrentConnectedDrone;
}

void UDroneControllerComponent::StartLaunchDrone(bool bConnectToDroneAfterLaunching)
{
	Server_StartLaunchDrone(bConnectToDroneAfterLaunching);
}

void UDroneControllerComponent::LaunchDrone()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ADroneBase* Drone = TryGetUnusedDrone();
		if (IsValid(Drone))
		{
			Drone->SetActorTickEnabled(true);
			Drone->SetActorTransform(GetComponentTransform());
			Drone->ShowDroneAndEnableCollision();
			Drone->SetIsActive(true);
			ActiveDrones.Add(Drone);

			SetAvailableDroneCount(AvailableDroneCount - 1);
		}
	}
}

void UDroneControllerComponent::ConnectToLastDrone()
{
	if (ActiveDrones.Num() == 0 && bShouldLaunchOnFailedConnection)
	{
		StartLaunchDrone(true);
		return;
	}
	
	if (GetOwnerRole() == ROLE_Authority && ActiveDrones.Num() > 0)
	{
		ADroneBase* Drone = ActiveDrones.Last();
		PossessDrone(CachedOwnerCharacter->GetController(), Drone);
	}

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_ConnectToLastDrone();
	}
}

bool UDroneControllerComponent::TryConnectToNextDrone()
{
	if (!IsValid(CurrentConnectedDrone) || !ActiveDrones.Contains(CurrentConnectedDrone))
	{
		return false;
	}

	int32 CurrentConnectedDroneIndex = ActiveDrones.IndexOfByKey(CurrentConnectedDrone);
	XHMathUtils::LoopIncrement(CurrentConnectedDroneIndex, ActiveDrones.Num());
	ADroneBase* NextDrone = ActiveDrones[CurrentConnectedDroneIndex];
	
	if (NextDrone != CurrentConnectedDrone)
	{
		PossessDrone(CurrentConnectedDrone->GetController(), NextDrone);
		return true;
	}
	
	return false;
}

bool UDroneControllerComponent::TryConnectToPreviousDrone()
{
	if (!IsValid(CurrentConnectedDrone) || !ActiveDrones.Contains(CurrentConnectedDrone))
	{
		return false;
	}

	int32 CurrentConnectedDroneIndex = ActiveDrones.IndexOfByKey(CurrentConnectedDrone);
	XHMathUtils::LoopDecrement(CurrentConnectedDroneIndex, -1, ActiveDrones.Num() - 1);
	ADroneBase* PreviousDrone = ActiveDrones[CurrentConnectedDroneIndex];

	if (PreviousDrone != CurrentConnectedDrone)
	{
		PossessDrone(CurrentConnectedDrone->GetController(), PreviousDrone);
		return true;
	}

	return false;
}

void UDroneControllerComponent::DisconnectDrone()
{
	if (IsValid(CurrentConnectedDrone))
	{
		PossessOwnerPawn(CurrentConnectedDrone->GetController());
	}
}

void UDroneControllerComponent::DeactivateDrone()
{
	if (!IsValid(CurrentConnectedDrone))
	{
		return;
	}
	
	CurrentConnectedDrone->HideDroneAndDisableCollision();
	CurrentConnectedDrone->SetIsActive(false);
	CurrentConnectedDrone->SetActorLocation(DronePoolLocation);
	ActiveDrones.Remove(CurrentConnectedDrone);
	
	SetAvailableDroneCount(AvailableDroneCount + 1);
	PossessOwnerPawn(CurrentConnectedDrone->GetController());
}

void UDroneControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ACharacter>(), TEXT("UDroneSpawnerComponent::BeginPlay DroneSpawnerComponent can be used only with a Character"));
	check(DroneClass);
	
	CachedOwnerCharacter = StaticCast<ACharacter*>(GetOwner());

	FillDronePool();

	SetAvailableDroneCount(MaxDroneCount);
}

void UDroneControllerComponent::StartLaunchDrone_Internal(bool bConnectToDroneAfterLaunching)
{
	if (!CanLaunch())
	{
		return;
	}

	if (!CachedOwnerCharacter.IsValid() || !IsValid(LaunchAnimMontage))
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			LaunchDrone();
			if (bConnectToDroneAfterLaunching)
			{
				ConnectToLastDrone();
			}
		}
	}
	else
	{
		float ConnectTime = CachedOwnerCharacter->PlayAnimMontage(LaunchAnimMontage);

		if ((GetOwnerRole() == ROLE_AutonomousProxy || CachedOwnerCharacter->IsLocallyControlled()) && bConnectToDroneAfterLaunching)
		{
			const FAnimNotifyEvent* LaunchDroneNotify = XHAnimUtils::GetAnimNotifyByClass<UAnimNotify_LaunchDrone>(LaunchAnimMontage);
			if (LaunchDroneNotify != nullptr)
			{
				ConnectTime = LaunchDroneNotify->GetEndTriggerTime();
			}
			
			GetWorld()->GetTimerManager().SetTimer(ConnectDroneTimer, this, &UDroneControllerComponent::ConnectToLastDrone, ConnectTime, false);
		}	
	}
}

void UDroneControllerComponent::FillDronePool()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		DronePool.Reserve(DronePoolSize);
	
		for (int32 i = 0; i < DronePoolSize; i++)
		{
			ADroneBase* Drone = GetWorld()->SpawnActor<ADroneBase>(DroneClass, DronePoolLocation, FRotator::ZeroRotator);
			if (IsValid(Drone))
			{
				Drone->SetActorTickEnabled(false);
				Drone->HideDroneAndDisableCollision();
				Drone->OnDroneDestroyed.AddUObject(this, &UDroneControllerComponent::OnDroneDestroyed);
				DronePool.Add(Drone);
			}
		}
	}
}

ADroneBase* UDroneControllerComponent::TryGetUnusedDrone()
{
	const int32 InitialIndex = CurrentDronePoolIndex;
	
	ADroneBase* Drone = nullptr;
	while (Drone == nullptr)
	{
		Drone = DronePool[CurrentDronePoolIndex];
		if (ActiveDrones.Contains(Drone))
		{
			Drone = nullptr;
		}
		
		XHMathUtils::LoopIncrement(CurrentDronePoolIndex, DronePoolSize);

		if (CurrentDronePoolIndex == InitialIndex)
		{
			break;
		}
	}

	return Drone;
}

bool UDroneControllerComponent::CanSpawnDrone() const
{
	if (!IsValid(DroneClass))
	{
		return false;
	}

	// Forward check
	
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

	const ADroneBase* DefaultDrone = DroneClass->GetDefaultObject<ADroneBase>();

	const float ForwardCheckCapsuleRadius = DefaultDrone->GetCapsuleComponent()->GetScaledCapsuleRadius() + RadiusSpawnOffset;
	const float ForwardCheckCapsuleHalfHeight = DefaultDrone->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + HeightSpawnOffset;

	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	
	const FVector EndLocation = GetComponentLocation();
	const FVector StartLocation = FVector(OwnerLocation.X, OwnerLocation.Y, EndLocation.Z);

	FHitResult HitResult;
	XHTraceUtils::SweepCapsuleSingleByChannel(GetWorld(), HitResult, StartLocation, EndLocation, ForwardCheckCapsuleRadius, ForwardCheckCapsuleHalfHeight, FQuat::Identity, ECC_Visibility, QueryParams, FCollisionResponseParams::DefaultResponseParam);

	return !HitResult.bBlockingHit;
}

void UDroneControllerComponent::OnDroneDestroyed(ADroneBase* DestroyedDrone)
{
	const bool bControllingDrone = IsControllingDroneNow();
	bool bPossesOnOwnerPawn = true;
	
	if (bControllingDrone && bShouldConnectToAvailableDroneOnDestroy)
	{
		bPossesOnOwnerPawn = !TryConnectToPreviousDrone();
	}

	DestroyedDrone->SetIsActive(false);
	DestroyedDrone->SetActorLocation(DronePoolLocation);
	ActiveDrones.Remove(DestroyedDrone);

	if (bControllingDrone && bPossesOnOwnerPawn)
	{
		PossessOwnerPawn(DestroyedDrone->GetController());
	}
}

void UDroneControllerComponent::PossessDrone(AController* Controller, ADroneBase* Drone)
{
	if (IsValid(Controller) && IsValid(Drone))
	{
		CurrentConnectedDrone = Drone;
		Controller->Possess(Drone);
	}
}

void UDroneControllerComponent::PossessOwnerPawn(AController* Controller)
{
	if (IsValid(Controller) && CachedOwnerCharacter.IsValid())
	{
		CurrentConnectedDrone = nullptr;
		Controller->Possess(CachedOwnerCharacter.Get());
	}
}

bool UDroneControllerComponent::CanLaunch() const
{
	return DronePool.Num() == DronePoolSize && AvailableDroneCount > 0 && CanSpawnDrone();
}

bool UDroneControllerComponent::IsControllingDroneNow() const
{
	return IsValid(CurrentConnectedDrone);
}

void UDroneControllerComponent::SetDroneWidgetVisibility(bool bWidgetVisible)
{
	if (!IsValid(DroneWidget))
	{
		check(IsValid(DroneWidgetClass));
		DroneWidget = CreateWidget<UDroneWidget>(GetWorld(), DroneWidgetClass);
	}

	if (bWidgetVisible)
	{
		DroneWidget->UpdateDroneBindings();
		if (!DroneWidget->IsInViewport())
		{
			DroneWidget->AddToViewport();
		}
	}
	else if (!bWidgetVisible && DroneWidget->IsInViewport())
	{
		DroneWidget->RemoveFromParent();
	}
}

void UDroneControllerComponent::OnRep_AvailableDroneCount()
{
	if (OnAvailableDroneCountChanges.IsBound())
	{
		OnAvailableDroneCountChanges.Broadcast(AvailableDroneCount);
	}
}

void UDroneControllerComponent::Server_StartLaunchDrone_Implementation(bool bConnectToDroneAfterLaunching)
{
	Multicast_StartLaunchDrone(bConnectToDroneAfterLaunching);
}

void UDroneControllerComponent::Multicast_StartLaunchDrone_Implementation(bool bConnectToDroneAfterLaunching)
{
	StartLaunchDrone_Internal(bConnectToDroneAfterLaunching);
}

void UDroneControllerComponent::Server_LaunchDrone_Implementation()
{
	LaunchDrone();
}

void UDroneControllerComponent::Server_ConnectToLastDrone_Implementation()
{
	ConnectToLastDrone();
}

void UDroneControllerComponent::Server_DisconnectDrone_Implementation()
{
	DisconnectDrone();
}

void UDroneControllerComponent::Server_ConnectToNextDrone_Implementation()
{
	TryConnectToNextDrone();
}

void UDroneControllerComponent::Server_ConnectToPreviousDrone_Implementation()
{
	TryConnectToPreviousDrone();
}

