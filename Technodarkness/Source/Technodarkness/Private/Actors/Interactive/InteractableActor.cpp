// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Actors/Interactive/InteractableActor.h"

void AInteractableActor::OnInteract_Implementation(AActor* WithInstigator)
{
	bWasInteracted = true;
	OnInteractDelegate.Broadcast(WithInstigator);
}
