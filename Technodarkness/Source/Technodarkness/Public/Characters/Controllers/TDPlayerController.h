// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Team/TDTeamId.h"
#include "TDPlayerController.generated.h"

UCLASS()
class TECHNODARKNESS_API ATDPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	
protected:
	// Player team id.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	ETeamId TeamId = ETeamId::None;
};
