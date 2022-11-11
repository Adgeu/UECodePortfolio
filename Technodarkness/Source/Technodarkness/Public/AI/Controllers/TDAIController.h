// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Team/TDTeamId.h"
#include "TDAIController.generated.h"

UCLASS()
class TECHNODARKNESS_API ATDAIController : public AAIController
{
	GENERATED_BODY()

	ATDAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	// Bot team id.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	ETeamId TeamId = ETeamId::None;
};
