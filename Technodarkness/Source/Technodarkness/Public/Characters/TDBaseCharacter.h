// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "TDBaseCharacter.generated.h"

class UCombinedSkeletalMeshComponent;

UCLASS()
class TECHNODARKNESS_API ATDBaseCharacter : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	ATDBaseCharacter(const FObjectInitializer& ObjectInitializer);

	/////////////////////////////////////////////////////////////////////

#pragma region Components

public:
	/** Returns CombinedMeshComponent subobject. */
	FORCEINLINE UCombinedSkeletalMeshComponent* GetCombinedMeshComponent() const
	{
		return CombinedMeshComponent;
	}
	
private:
	// The main skeletal mesh combined from separate skeletal meshes associated with this Character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCombinedSkeletalMeshComponent* CombinedMeshComponent;

#pragma endregion ~Components

	/////////////////////////////////////////////////////////////////////

#pragma region IAISightTargetInterface
	
public:
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const override;

protected:
	// Should we use sight check with bones filter.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Sight Target")
	bool bUseSightCheckFilterBoneNames = true;

	// Sight check bones filter.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Sight Target", meta = (EditCondition = "bUseSightCheckFilterBoneNames", EditConditionHides))
	TArray<FName> SightCheckFilterBoneNames;
	
private:
	bool CanBeSeenFrom_Internal(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const FVector& TargetLocation, const AActor* IgnoreActor) const;

#pragma endregion ~IAISightTargetInterface
};
