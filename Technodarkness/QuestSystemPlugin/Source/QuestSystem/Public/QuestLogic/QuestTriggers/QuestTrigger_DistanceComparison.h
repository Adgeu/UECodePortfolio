// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestTrigger.h"
#include "QuestTrigger_DistanceComparison.generated.h"

///////////////////////////////////////////////////////

/**
 * Distance comparison operators.
 */
UENUM()
enum class EDistanceComparisonOperator : uint8
{
	Equal				UMETA(DisplayName = "==", ToolTip = "Equal"),
	NotEqual			UMETA(DisplayName = "!=", ToolTip = "Not equal"),
	Greater				UMETA(DisplayName = ">", ToolTip = "Greater"),
	GreaterOrEqual		UMETA(DisplayName = ">=", ToolTip = "Greater or equal"),
	Less				UMETA(DisplayName = "<", ToolTip = "Less"),
	LessOrEqual			UMETA(DisplayName = "<=", ToolTip = "Less or equal")
};

///////////////////////////////////////////////////////

/**
 * Comparison target types.
 */
UENUM()
enum class EDistanceComparisonTarget : uint8
{
	// The target of the comparison will be the player.
	Player,

	// The target of the comparison will be the actor, that implements IQuestTarget interface.
	QuestTarget,

	// The target of the comparison will be the raw vector.
	Location
};

///////////////////////////////////////////////////////

/**
 * Enters in triggered state when the distance between two targets satisfies the comparison condition.
 */
UCLASS()
class QUESTSYSTEM_API UQuestTrigger_DistanceComparison : public UQuestTrigger
{
	GENERATED_BODY()

public:
	UQuestTrigger_DistanceComparison();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnTrackingBegins_Implementation() override;
	virtual void OnTrackingEnded_Implementation() override;
	
	// Operator for comparison operation.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	EDistanceComparisonOperator DistanceComparisonOperator = EDistanceComparisonOperator::Equal;

	// Check distance between two targets in the XY plane only.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings", meta = (DisplayName="Check Only XY Plane"))
	bool bCheckOnlyXYPlane = false;

	// Distance that we want to check.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings", meta = (ClampMin = 0.0, ForceUnits = "cm"))
	float DistanceToCheck = 0.0f;

	// Distance threshold.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings", meta = (ClampMin = 0.0, ForceUnits = "cm"))
	float DistanceCheckThreshold = KINDA_SMALL_NUMBER;

	// Type of the first comparison target.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings|First Target")
	EDistanceComparisonTarget FirstTargetType = EDistanceComparisonTarget::Player;

	// Id for the first actor target. Target must implement IQuestTarget interface.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings|First Target", meta = (EditCondition = "FirstTargetType == EDistanceComparisonTarget::QuestTarget", EditConditionHides))
	FGameplayTag FirstTargetId;

	// Location for the first target in world space.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings|First Target", meta = (EditCondition = "FirstTargetType == EDistanceComparisonTarget::Location", EditConditionHides))
	FVector FirstTargetWorldLocation = FVector::ZeroVector;

	// Type of the second comparison target.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings|Second Target")
	EDistanceComparisonTarget SecondTargetType = EDistanceComparisonTarget::Location;

	// Id for the second actor target. Target must implement IQuestTarget interface.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings|Second Target", meta = (EditCondition = "SecondTargetType == EDistanceComparisonTarget::QuestTarget", EditConditionHides))
	FGameplayTag SecondTargetId;

	// Location for the second target in world space.
	UPROPERTY(EditAnywhere, Category = "Trigger Settings|Second Target", meta = (EditCondition = "SecondTargetType == EDistanceComparisonTarget::Location", EditConditionHides))
	FVector SecondTargetWorldLocation = FVector::ZeroVector;

private:
	/** Returns squared distance between targets */
	float GetSquaredDistanceBetweenTargets() const;
	
	/** Returns location of the target. */
	FVector GetTargetLocation(EDistanceComparisonTarget TargetType, const AActor* TargetActor, const FVector& TargetWorldLocation) const;

	/** Returns location of the second target based on first target type. */
	FORCEINLINE FVector GetFirstTargetLocation() const
	{
		return GetTargetLocation(FirstTargetType, FirstTargetActor, FirstTargetWorldLocation);
	}

	/** Returns location of the second target based on second target type. */
	FORCEINLINE FVector GetSecondTargetLocation() const
	{
		return GetTargetLocation(SecondTargetType, SecondTargetActor, SecondTargetWorldLocation);
	}

	/** Based on the comparison result decides whether the condition is met, undo it or pass. */
	FORCEINLINE void HandleComparisonResult(bool bComparisonResult)
	{
		if (bComparisonResult)
		{
			EnterTriggeredState();
		}
		else
		{
			if (IsTriggered())
			{
				UndoTriggeredState();
			}
		}
	}

	// Cached player character.
	UPROPERTY()
	ACharacter* PlayerCharacter;

	// Cached first target actor.
	UPROPERTY()
	AActor* FirstTargetActor;
	
	// Cached second target actor.
	UPROPERTY()
	AActor* SecondTargetActor;
};
