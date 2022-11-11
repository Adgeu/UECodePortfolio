// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLocationMarker.h"
#include "QuestLocationSphereMarker.generated.h"

/**
 * Location marker for quest objectives condition targets with sphere overlapping volume.
 */
UCLASS()
class TECHNODARKNESS_API AQuestLocationSphereMarker : public AQuestLocationMarker
{
	GENERATED_BODY()

public:
	AQuestLocationSphereMarker();
};
