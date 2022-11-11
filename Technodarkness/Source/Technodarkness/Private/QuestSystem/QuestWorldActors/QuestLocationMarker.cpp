// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "QuestSystem/QuestWorldActors/QuestLocationMarker.h"

#include "Characters/TDPlayerCharacter.h"

FGameplayTag AQuestLocationMarker::GetQuestTargetId_Implementation() const
{
	return QuestTargetId;
}

void AQuestLocationMarker::AddOverlappingCharacter(ACharacter* NewCharacter)
{
	if (NewCharacter)
	{
		CharactersForOverlapping.Add(NewCharacter);
	}
}

void AQuestLocationMarker::RemoveOverlappingCharacter(ACharacter* NewCharacter)
{
	if (NewCharacter)
	{
		CharactersForOverlapping.RemoveSwap(NewCharacter);
	}
}

void AQuestLocationMarker::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(OverlappingVolume))
	{
		OverlappingVolume->OnComponentBeginOverlap.AddDynamic(this, &AQuestLocationMarker::OnVolumeOverlapBegin);
		OverlappingVolume->OnComponentEndOverlap.AddDynamic(this, &AQuestLocationMarker::OnVolumeOverlapEnd);
	}
}

void AQuestLocationMarker::OnVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (ACharacter* Character : CharactersForOverlapping)
	{
		if (Character == OtherActor)
		{
			OnBeginOverlapWithCharacter.Broadcast(Character);
		}
	}
}

void AQuestLocationMarker::OnVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (ACharacter* Character : CharactersForOverlapping)
	{
		if (Character == OtherActor)
		{
			OnEndOverlapWithCharacter.Broadcast(Character);
		}
	}
}
