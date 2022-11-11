// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Characters/TDBaseCharacter.h"

#include "Components/SkeletalMeshComponents/CombinedSkeletalMeshComponent.h"


ATDBaseCharacter::ATDBaseCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCombinedSkeletalMeshComponent>(ACharacter::MeshComponentName))
{
	CombinedMeshComponent = CastChecked<UCombinedSkeletalMeshComponent>(GetMesh());
}

bool ATDBaseCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const
{
	if (bUseSightCheckFilterBoneNames)
	{
		for (const FName& Socket : SightCheckFilterBoneNames)
		{
			FVector SocketLocation = GetMesh()->GetSocketLocation(Socket);
			if (CanBeSeenFrom_Internal(ObserverLocation, OutSeenLocation, NumberOfLoSChecksPerformed, OutSightStrength, SocketLocation, IgnoreActor))
			{
				return true;
			}
		}
	}
	else
	{
		if (CanBeSeenFrom_Internal(ObserverLocation, OutSeenLocation, NumberOfLoSChecksPerformed, OutSightStrength, GetActorLocation(), IgnoreActor))
		{
			return true;
		}
	}

	OutSightStrength = 0;
	return false;
}

bool ATDBaseCharacter::CanBeSeenFrom_Internal(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const FVector& TargetLocation, const AActor* IgnoreActor) const
{
	FHitResult HitResult;

	FCollisionObjectQueryParams ObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic));
	// TODO define custom trace channel
	FCollisionQueryParams QueryParams(FName("Visible"), true, IgnoreActor); 
	const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, TargetLocation, ObjectQueryParams, QueryParams);

	++NumberOfLoSChecksPerformed;

	if (!bHitSocket || HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this))
	{
		OutSeenLocation = TargetLocation;
		OutSightStrength = 1;

		return true;
	}

	return false;
}
