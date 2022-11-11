// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "AI/Controllers/TDAIController.h"
#include "Navigation/CrowdFollowingComponent.h"


ATDAIController::ATDAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{

}

FGenericTeamId ATDAIController::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(TeamId));
}

ETeamAttitude::Type ATDAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			const ETeamId OtherTeamId = static_cast<ETeamId>(TeamAgent->GetGenericTeamId().GetId());
			return FTeamIdUtils::GetTeamAttitude(TeamId, OtherTeamId);
		}
	}

	return ETeamAttitude::Neutral;
}

