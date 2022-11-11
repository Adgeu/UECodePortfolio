// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/QuestTarget.h"
#include "QuestLocationMarker.generated.h"

class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginOverlapWithCharacter, ACharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndOverlapWithCharacter, ACharacter*, Character);

/**
 * Location marker for quest objectives condition targets.
 */
UCLASS()
class TECHNODARKNESS_API AQuestLocationMarker : public AActor, public IQuestTarget
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetQuestTargetId_Implementation() const override;

	/** Add character for tracking overlap with this marker. */
	UFUNCTION(BlueprintCallable)
	void AddOverlappingCharacter(ACharacter* NewCharacter);

	/** Remove character for tracking overlap with this marker. */
	UFUNCTION(BlueprintCallable)
	void RemoveOverlappingCharacter(ACharacter* NewCharacter);

	/** Returns overlapping volume subobject. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UPrimitiveComponent* GetOverlappingVolume() const
	{
		return OverlappingVolume;
	}

	// Notify that the character from the CharactersForOverlapping list is begin overlapping with the marker volume.
	UPROPERTY(BlueprintAssignable)
	FOnBeginOverlapWithCharacter OnBeginOverlapWithCharacter;

	// Notify that the character from the CharactersForOverlapping list is end overlapping with the marker volume.
	UPROPERTY(BlueprintAssignable)
	FOnEndOverlapWithCharacter OnEndOverlapWithCharacter;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Volume with which we want to track overlapping with the characters.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overlapping")
	class UPrimitiveComponent* OverlappingVolume;

	// Id for connecting with the quest system.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Objective Target")
	FGameplayTag QuestTargetId;

private:
	// The list of the characters with which we want to track overlapping.
	UPROPERTY()
	TArray<ACharacter*> CharactersForOverlapping;
};
