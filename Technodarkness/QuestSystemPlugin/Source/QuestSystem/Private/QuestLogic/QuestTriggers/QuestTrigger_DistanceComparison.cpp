// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "QuestLogic/QuestTriggers/QuestTrigger_DistanceComparison.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Types/QuestTypes.h"

UQuestTrigger_DistanceComparison::UQuestTrigger_DistanceComparison()
{
	bTickableTrigger = true;
}

void UQuestTrigger_DistanceComparison::Tick(float DeltaTime)
{
	const float SquaredDistance = GetSquaredDistanceBetweenTargets();
	const float SquaredDistanceToCheck = DistanceToCheck * DistanceToCheck;

	switch (DistanceComparisonOperator)
	{
		case EDistanceComparisonOperator::Equal:
			HandleComparisonResult(FMath::IsNearlyEqual(SquaredDistance, SquaredDistanceToCheck, DistanceCheckThreshold));
			break;

		case EDistanceComparisonOperator::NotEqual:
			HandleComparisonResult(!FMath::IsNearlyEqual(SquaredDistance, SquaredDistanceToCheck, DistanceCheckThreshold));
			break;

		case EDistanceComparisonOperator::Greater:
			HandleComparisonResult(SquaredDistance - DistanceCheckThreshold > SquaredDistanceToCheck);
			break;

		case EDistanceComparisonOperator::GreaterOrEqual:
			HandleComparisonResult(SquaredDistance - DistanceCheckThreshold >= SquaredDistanceToCheck);
			break;

		case EDistanceComparisonOperator::Less:
			HandleComparisonResult(SquaredDistance + DistanceCheckThreshold < SquaredDistanceToCheck);
			break;

		case EDistanceComparisonOperator::LessOrEqual:
			HandleComparisonResult(SquaredDistance + DistanceCheckThreshold <= SquaredDistanceToCheck);
			break;
	}
}

void UQuestTrigger_DistanceComparison::OnTrackingBegins_Implementation()
{
	if (FirstTargetType == EDistanceComparisonTarget::Player || SecondTargetType == EDistanceComparisonTarget::Player)
	{
		PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	
	if (FirstTargetType == EDistanceComparisonTarget::QuestTarget)
	{
		FirstTargetActor = FindQuestTargetActorWithId(FirstTargetId);
	}

	if (SecondTargetType == EDistanceComparisonTarget::QuestTarget)
	{
		SecondTargetActor = FindQuestTargetActorWithId(SecondTargetId);
	}
}

void UQuestTrigger_DistanceComparison::OnTrackingEnded_Implementation()
{
	PlayerCharacter = nullptr;
	FirstTargetActor = nullptr;
	SecondTargetActor = nullptr;
}

float UQuestTrigger_DistanceComparison::GetSquaredDistanceBetweenTargets() const
{
	const FVector FirstTargetLocation = GetFirstTargetLocation();
	const FVector SecondTargetLocation = GetSecondTargetLocation();

	return bCheckOnlyXYPlane ? FVector::DistSquaredXY(FirstTargetLocation, SecondTargetLocation) : FVector::DistSquared(FirstTargetLocation, SecondTargetLocation);
}

FVector UQuestTrigger_DistanceComparison::GetTargetLocation(EDistanceComparisonTarget TargetType, const AActor* TargetActor, const FVector& TargetWorldLocation) const
{
	FVector Result = FVector::ZeroVector;
	
	switch (TargetType)
	{
		case EDistanceComparisonTarget::Player:
			if (PlayerCharacter)
			{
				Result = PlayerCharacter->GetActorLocation();
			}
			else
			{
				UE_LOG(LogQuest, Error, TEXT("%s() Player is not valid"), *FString(__FUNCTION__));
			}
			break;

		case EDistanceComparisonTarget::QuestTarget:
			if (TargetActor)
			{
				Result = TargetActor->GetActorLocation();
			}
			else
			{
				UE_LOG(LogQuest, Error, TEXT("%s() Target actor is not valid"), *FString(__FUNCTION__));
			}
			break;

		case EDistanceComparisonTarget::Location:
			Result = TargetWorldLocation;
			break;
	}

	return Result;
}
