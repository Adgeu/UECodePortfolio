// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestSystem/QuestWorldActors/QuestLocationSphereMarker.h"

#include "Components/SphereComponent.h"
#include "Technodarkness/TDTypes.h"

AQuestLocationSphereMarker::AQuestLocationSphereMarker()
{
	OverlappingVolume = CreateDefaultSubobject<USphereComponent>("OverlappingVolume");
	OverlappingVolume->SetCollisionProfileName(CollisionProfileTrigger);
	SetRootComponent(OverlappingVolume);
}
