// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "QuestTrigger_OnTakeDamage.generated.h"

UENUM()
enum class ETakeDamageActorType : uint8
{
	// Actor is player character.
	Player,
	// Actor that implements IQuestTarget interface.
	QuestTargetActor
};

/**
 * Enters in triggered state when the actor take any damage.
 */
UCLASS()
class QUESTSYSTEM_API UQuestTrigger_OnTakeDamage : public UQuestTrigger
{
	GENERATED_BODY()

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;
	
	// Should we filter taking damage by instigator or any damage can trigger us.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instigator")
	bool bShouldFilterByInstigator = false;

	// Type of the instigator actor.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instigator", meta = (EditCondition = "bShouldFilterByInstigator", EditConditionHides))
	ETakeDamageActorType InstigatorType = ETakeDamageActorType::Player;

	// Id of the instigator actor. Actor must implement IQuestTarget interface.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instigator", meta = (EditCondition = "bShouldFilterByInstigator && InstigatorType == ETakeDamageActorType::QuestTargetActor", EditConditionHides))
	FGameplayTag InstigatorTargetId;

	// Type of the victim actor.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Victim")
	ETakeDamageActorType VictimType = ETakeDamageActorType::QuestTargetActor;

	// Id of the victim actor. Actor must implement IQuestTarget interface.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Victim", meta = (EditCondition = "VictimType == ETakeDamageActorType::QuestTargetActor", EditConditionHides))
	FGameplayTag VictimTargetId;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Cached instigator actor if we want to filter by instigator.
	UPROPERTY()
	AActor* InstigatorActor = nullptr;

	// Cached victim actor.
	UPROPERTY()
	AActor* VictimActor = nullptr;
};
