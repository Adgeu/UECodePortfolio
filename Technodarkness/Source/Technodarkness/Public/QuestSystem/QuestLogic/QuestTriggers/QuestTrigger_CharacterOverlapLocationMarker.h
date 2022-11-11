// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestLogic/QuestTriggers/QuestTrigger.h"
#include "QuestTrigger_CharacterOverlapLocationMarker.generated.h"

class ACharacter;

// Specifying when we should track location marker volume overlapping.
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ELocationMarkerOverlapType : uint8
{
	// Do not track overlap.
	None	= 0 UMETA(Hidden),

	// Track for the character entering overlapping volume.
	Enter	= 1 << 0,

	// Track for the character exiting overlapping volume.
	Exit	= 1 << 1,

	// Track for the character entering and exiting overlapping volume.
	All = Enter | Exit
};
ENUM_CLASS_FLAGS(ELocationMarkerOverlapType);

///////////////////////////////////////////////////////

/**
 * Enters in triggered state when the character is begin or end (or both) overlap with the marker volume.
 * Use ConditionTargetId to specify the location marker.
 */
UCLASS()
class TECHNODARKNESS_API UQuestTrigger_CharacterOverlapLocationMarker : public UQuestTrigger
{
	GENERATED_BODY()

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;
	
	// We can choose when the condition is met: when the character enter overlap volume, exit overlap volume or both.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Settings", meta = (Bitmask, BitmaskEnum = ELocationMarkerOverlapType))
	uint8 TrackOverlappingOn = static_cast<uint8>(ELocationMarkerOverlapType::All);

	// Track overlap with the player (true) or with other characters (false)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Settings")
	bool bOverlapWithPlayer = true;

	// Other character target id. The character should implements IQuestTarget interface.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Settings", meta = (EditCondition = "!bOverlapWithPlayer", EditConditionHides))
	FGameplayTag OverlapCharacterId;

	// Target Id of the location marker.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Settings")
	FGameplayTag LocationMarkerTargetId;
	
private:
	UFUNCTION()
	void OnBeginOverlapWithCharacter(ACharacter* Character);

	UFUNCTION()
	void OnEndOverlapWithCharacter(ACharacter* Character);

	FORCEINLINE bool HasEnterFlag() const
	{
		return TrackOverlappingOn & static_cast<uint8>(ELocationMarkerOverlapType::Enter);
	}

	FORCEINLINE bool HasExitFlag() const
	{
		return TrackOverlappingOn & static_cast<uint8>(ELocationMarkerOverlapType::Exit);
	}
	
	UPROPERTY()
	class AQuestLocationMarker* TargetLocationMarker;
};
