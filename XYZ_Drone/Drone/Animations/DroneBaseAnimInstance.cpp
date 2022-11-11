// For educational purposes only. 


#include "Drone/Animations/DroneBaseAnimInstance.h"

#include "Drone/DroneBase.h"

void UDroneBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<ADroneBase>(), TEXT("UDroneBaseAnimInstance::NativeBeginPlay UDroneBaseAnimInstance can be used only with ADroneBase" ));
	CachedDroneBase = StaticCast<ADroneBase*>(TryGetPawnOwner());
}

void UDroneBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedDroneBase.IsValid())
	{
		return;
	}

	bIsWorking = CachedDroneBase->IsActive();
}