// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLocationMarker.h"
#include "QuestLocationBoxMarker.generated.h"

/**
 * Location marker for quest objectives condition targets with box overlapping volume.
 */
UCLASS()
class TECHNODARKNESS_API AQuestLocationBoxMarker : public AQuestLocationMarker
{
	GENERATED_BODY()

public:
	AQuestLocationBoxMarker();
};
