// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractDelegate, class AActor*, Instigator);

/**
 * Base class for all interactable actors.
 */
UCLASS(Abstract)
class TECHNODARKNESS_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	// Someone interacted with this actor.
	UPROPERTY(BlueprintAssignable)
	FOnInteractDelegate OnInteractDelegate;

	/** This actor was interacted at least once. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool WasInteracted() const
	{
		return bWasInteracted;
	}
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteract(AActor* WithInstigator);

private:
	// This actor was interacted at least once.
	bool bWasInteracted = false;
};
