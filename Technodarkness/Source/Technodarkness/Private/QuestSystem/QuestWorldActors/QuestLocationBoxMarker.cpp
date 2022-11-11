// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestSystem/QuestWorldActors/QuestLocationBoxMarker.h"

#include "Components/BoxComponent.h"
#include "Technodarkness/TDTypes.h"

AQuestLocationBoxMarker::AQuestLocationBoxMarker()
{
	OverlappingVolume = CreateDefaultSubobject<UBoxComponent>("OverlappingVolume");
	OverlappingVolume->SetCollisionProfileName(CollisionProfileTrigger);
	SetRootComponent(OverlappingVolume);
}
