// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "AI/Characters/TDAICharacter.h"

void ATDAICharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (const USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		OutLocation = MeshComponent->GetSocketLocation(EyesViewPointBoneName);
	
		OutRotation = GetActorRotation();
		OutRotation.Yaw -= MeshComponent->GetSocketTransform(EyesViewPointBoneName, RTS_ParentBoneSpace).Rotator().Roll;
	}
	else
	{
		Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	}
}
