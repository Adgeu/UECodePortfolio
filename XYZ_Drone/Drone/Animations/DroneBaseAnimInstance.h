// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DroneBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XYZHOMEWORK_API UDroneBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone animation")
	bool bIsWorking = false;

private:
	TWeakObjectPtr<class ADroneBase> CachedDroneBase;
};
