// For educational purposes only. 


#include "Drone/DroneBase.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterComponents/HealthComponent.h"
#include "Components/Weapon/ExplosionComponent.h"
#include "Engine/CollisionProfile.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"

ADroneBase::ADroneBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCylinder"));
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SetRootComponent(CapsuleComponent);
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetCollisionProfileName(CollisionProfileCharacterMesh);
	MeshComponent->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	ExplosionComponent = CreateDefaultSubobject<UExplosionComponent>(TEXT("ExplosionComponent"));
	ExplosionComponent->SetupAttachment(GetRootComponent());

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->SetUpdatedComponent(CapsuleComponent);
	FloatingPawnMovement->MaxSpeed = 300.0f;
	FloatingPawnMovement->Acceleration = 2000.0f;
	FloatingPawnMovement->Deceleration = 500.0f;
	FloatingPawnMovement->TurningBoost = 1.0f;
	
#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->ArrowColor = FColor(150, 200, 255);
	ArrowComponent->bTreatAsASprite = true;
	ArrowComponent->SetupAttachment(GetRootComponent());
	ArrowComponent->bIsScreenSizeScaled = true;
#endif

	SetReplicates(true);
	SetReplicatingMovement(true);
}

void ADroneBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() == ROLE_AutonomousProxy || IsLocallyControlled())
	{
		Server_UpdatePawnTransform(GetActorTransform());
		TiltDrone(DeltaSeconds);
	}
	
	UpdateInputAcceleration();
	UpdateVelocity();
	CalculateAltitude();
}

void ADroneBase::UnPossessed()
{
	Super::UnPossessed();
	FloatingPawnMovement->StopMovementImmediately();
}

void ADroneBase::SelfDestruct()
{
	Server_SelfDestruct();
}

void ADroneBase::MoveForward(float Value)
{
	ForwardAxis = Value;
}

void ADroneBase::MoveRight(float Value)
{
	RightAxis = Value;
}

void ADroneBase::MoveUp(float Value)
{
	UpAxis = Value;
}

void ADroneBase::Turn(float Value)
{
}

void ADroneBase::LookUp(float Value)
{
}

void ADroneBase::TurnAtRate(float Value)
{
}

void ADroneBase::LookUpAtRate(float Value)
{
}

void ADroneBase::ShowDroneAndEnableCollision()
{
	MeshComponent->SetVisibility(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ADroneBase::HideDroneAndDisableCollision()
{
	MeshComponent->SetVisibility(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADroneBase::BeginPlay()
{
	Super::BeginPlay();

	check(MeshComponent);
	check(HealthComponent);
	check(ExplosionComponent);
	check(FloatingPawnMovement);

	HealthComponent->OnDeathEvent.AddUObject(this, &ADroneBase::OnDroneDeath);
}

void ADroneBase::OnDroneDeath()
{
	Server_OnDroneDeath();
}

void ADroneBase::TiltDrone(float DeltaSeconds)
{
	const FRotator Rotation = MeshComponent->GetRelativeRotation();
	const FRotator TargetRotation = FRotator(MaxTiltPitchAngle * -ForwardAxis, Rotation.Yaw, MaxTiltRollAngle * RightAxis);
	const FRotator NewRotation = FMath::RInterpTo(Rotation, TargetRotation, DeltaSeconds, TiltSpeed);
	MeshComponent->SetRelativeRotation(NewRotation);

	if (!FMath::IsNearlyEqual(Rotation.Roll, NewRotation.Roll) && OnRollAngleChanges.IsBound())
	{
		OnRollAngleChanges.Broadcast(NewRotation.Roll);
	}

	if (GetRemoteRole() == ROLE_AutonomousProxy)
	{
		Server_UpdateMeshTilt(NewRotation);
	}
}

void ADroneBase::UpdateDroneInfo()
{
	UpdateInputAcceleration(true);
	UpdateVelocity(true);
	CalculateAltitude(true);
}

void ADroneBase::CalculateAltitude(bool bForceBroadcast)
{
	if (!IsPawnControlled() && bDisableLineTraceOnUnPossess)
	{
		return;
	}
	
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector::UpVector * MaxAltitudeLineDistance;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	FHitResult HitResult;
	const bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
	const float NewAltitude = bResult ? HitResult.Distance - CapsuleComponent->GetUnscaledCapsuleHalfHeight() : -1.0f;

	if ((bForceBroadcast || !FMath::IsNearlyEqual(CurrentAltitude, NewAltitude)) && OnAltitudeChanges.IsBound())
	{
		OnAltitudeChanges.Broadcast(bResult, NewAltitude);
	}
	
	CurrentAltitude = NewAltitude;
}

void ADroneBase::UpdateInputAcceleration(bool bForceBroadcast)
{
	const float ForwardAxisAbs = FMath::Abs(ForwardAxis);
	const float RightAxisAbs = FMath::Abs(RightAxis);
	const float UpAxisAbs = FMath::Abs(UpAxis);
	const float NewAcceleration = FMath::Clamp(ForwardAxisAbs + RightAxisAbs + UpAxisAbs, 0.0f, 1.0f);
	if ((bForceBroadcast || !FMath::IsNearlyEqual(CurrentAcceleration, NewAcceleration)) && OnInputAccelerationChanges.IsBound())
	{
		OnInputAccelerationChanges.Broadcast(NewAcceleration);
	}
	CurrentAcceleration = NewAcceleration;
}

void ADroneBase::UpdateVelocity(bool bForceBroadcast)
{
	const float NewVelocity = FloatingPawnMovement->Velocity.Size();
	if ((bForceBroadcast || !FMath::IsNearlyEqual(CurrentVelocity, NewVelocity)) && OnSpeedChanges.IsBound())
	{
		OnSpeedChanges.Broadcast(NewVelocity, NewVelocity / FloatingPawnMovement->GetMaxSpeed());
	}
	CurrentVelocity = NewVelocity;
}

FGenericTeamId ADroneBase::GetGenericTeamId() const
{
	return FGenericTeamId((uint8)Team);
}

void ADroneBase::Server_UpdatePawnTransform_Implementation(const FTransform& NewTransform)
{
	SetActorTransform(NewTransform);
}

void ADroneBase::Server_UpdateMeshTilt_Implementation(const FRotator& NewRotation)
{
	Multicast_UpdateMeshTilt(NewRotation);
}

void ADroneBase::Multicast_UpdateMeshTilt_Implementation(const FRotator& NewRotation)
{
	if (GetRemoteRole() != ROLE_AutonomousProxy)
	{
		MeshComponent->SetRelativeRotation(NewRotation);
	}
}

void ADroneBase::Server_SelfDestruct_Implementation()
{
	Multicast_SelfDestruct();
}

void ADroneBase::Multicast_SelfDestruct_Implementation()
{
	HideDroneAndDisableCollision();
	
	ExplosionComponent->Explode(GetController());
	
	if (OnDroneDestroyed.IsBound())
	{
		OnDroneDestroyed.Broadcast(this);
	}
}

void ADroneBase::Server_OnDroneDeath_Implementation()
{
	Multicast_OnDroneDeath();
}

void ADroneBase::Multicast_OnDroneDeath_Implementation()
{
	HideDroneAndDisableCollision();

	if (IsValid(DeathVFX))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathVFX, GetActorLocation());
	}
	
	if (OnDroneDestroyed.IsBound())
	{
		OnDroneDestroyed.Broadcast(this);
	}
}
