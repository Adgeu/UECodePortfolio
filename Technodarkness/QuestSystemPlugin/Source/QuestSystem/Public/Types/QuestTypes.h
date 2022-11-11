#pragma once

#include "QuestTypes.generated.h"

///////////////////////////////////////////////////////

// Log category for quest system.
DEFINE_LOG_CATEGORY_STATIC(LogQuest, All, All);

///////////////////////////////////////////////////////

/** The category of the quest. */
UENUM(BlueprintType)
enum class EQuestCategory : uint8
{
	// The quest has no category.
	None,
	
	// Main quest.
	Main,

	// Side quest.
	Side
};

///////////////////////////////////////////////////////

/**
 * Current quest or objective running state.
 */
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	// Quest or objective is not active and waits to start.
	Inactive,

	// Quest or objective in progress.
	InProgress,

	// Quest or objective was completed (successful or failed).
	Completed
};

///////////////////////////////////////////////////////

/**
 * Type that describes how quest items were completed.
 * QuestCompleteType used for both quests and objectives.
 */
UENUM(BlueprintType)
enum class EQuestCompletionType : uint8
{
	// Quest or objective is not completed.
	None		UMETA(Hidden),

	// Quest or objective was completed with success.
	Success,

	// Quest or objective was completed with fail.
	Fail
};

///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FLoreDisplayingElement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Caption;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D> Texture;
};

USTRUCT(BlueprintType)
struct FLoreItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LocalizationKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLoreDisplayingElement> DisplayingElements;
};

USTRUCT(BlueprintType)
struct FLoreContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "LocalizationKey"))
	TArray<FLoreItem> LoreItems;

	bool HasLocalizationKey(const FString& Key) const;
	const FLoreItem& FindItemByLocalizationKey(const FString& Key) const;

	TArray<FLoreDisplayingElement> GetDisplayingElements(const FString& LocalizationKey) const;
	FLoreDisplayingElement GetDisplayingElement(const FString& LocalizationKey, int32 ElementIndex) const;

	FORCEINLINE const FLoreItem& operator[](const FString LocalizationKey) const
	{
		return this->FindItemByLocalizationKey(LocalizationKey);
	}
};

///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FDocURL
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString URL;
};

///////////////////////////////////////////////////////
