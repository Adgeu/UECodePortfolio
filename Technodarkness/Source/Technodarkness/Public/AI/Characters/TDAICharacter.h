// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Characters/TDBaseCharacter.h"
#include "TDAICharacter.generated.h"

UCLASS()
class TECHNODARKNESS_API ATDAICharacter : public ATDBaseCharacter
{
	GENERATED_BODY()

public:
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

protected:
	// Bone name for attaching eyes view point.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eyes View Point")
	FName EyesViewPointBoneName = NAME_None;
};
