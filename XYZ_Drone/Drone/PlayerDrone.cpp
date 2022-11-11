// For educational purposes only. 


#include "Drone/PlayerDrone.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

APlayerDrone::APlayerDrone()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetMeshComponent());
}

void APlayerDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CalculateDistance();
}

void APlayerDrone::MoveForward(float Value)
{
	Super::MoveForward(Value);
	GetFloatingPawnMovement()->AddInputVector(GetActorForwardVector() * Value);
}

void APlayerDrone::MoveRight(float Value)
{
	Super::MoveRight(Value);
	GetFloatingPawnMovement()->AddInputVector(GetActorRightVector() * Value);
}

void APlayerDrone::MoveUp(float Value)
{
	Super::MoveUp(Value);
	GetFloatingPawnMovement()->AddInputVector(GetActorUpVector() * Value);
}

void APlayerDrone::Turn(float Value)
{
	Super::Turn(Value);
	AddActorWorldRotation(FRotator(0.0f, Value, 0.0f));
}

void APlayerDrone::LookUp(float Value)
{
	Super::LookUp(Value);
	RotateCameraPitch(Value);
}

void APlayerDrone::TurnAtRate(float Value)
{
	Super::TurnAtRate(Value);
	AddActorWorldRotation(FRotator(0.0f, Value * BaseTurnRate * GetWorld()->GetDeltaSeconds(), 0.0f));
}

void APlayerDrone::LookUpAtRate(float Value)
{
	Super::LookUpAtRate(Value);
	RotateCameraPitch(Value * BaseLookUpRate);
}

void APlayerDrone::CalculateDistance(bool bForceBroadcast)
{
	if (!IsPawnControlled() && bDisableLineTraceOnUnPossess)
	{
		return;
	}

	const FVector Start = GetActorLocation();
	const FVector End = Start + CameraComponent->GetForwardVector() * MaxAltitudeLineDistance;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	const bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);

	const float NewDistance = bResult ? HitResult.Distance - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() : -1.0f;

	if ((bForceBroadcast || !FMath::IsNearlyEqual(CurrentDistance, NewDistance)) && OnDistanceChanges.IsBound())
	{
		OnDistanceChanges.Broadcast(bResult, NewDistance);
	}

	CurrentDistance = NewDistance;
}

void APlayerDrone::UpdateDroneInfo()
{
	Super::UpdateDroneInfo();
	CalculateDistance(true);
}

void APlayerDrone::RotateCameraPitch(float Value)
{
	const FRotator Rotation = CameraComponent->GetRelativeRotation();
	const float NewPitch = FMath::Clamp(Rotation.Pitch + Value, MinCameraPitch, MaxCameraPitch);
	CameraComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
}
