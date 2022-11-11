// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Interfaces/QuestTarget.h"
#include "ActorWithPlaceableMeshes.generated.h"

/**
 * Actor with possibles to place or remove static meshes.
 */
UCLASS(Abstract)
class TECHNODARKNESS_API AActorWithPlaceableMeshes : public AInteractableActor, public IQuestTarget
{
	GENERATED_BODY()

public:
	/** Actor should add static meshes. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnShouldAddStaticMeshes(const TArray<UStaticMesh*>& StaticMeshesToAdd);

	/** Actor should remove static meshes. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnShouldRemoveStaticMeshes(const TArray<UStaticMesh*>& StaticMeshesToRemove);
	
	virtual FGameplayTag GetQuestTargetId_Implementation() const override;
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Quest")
	FGameplayTag QuestTargetId;
};
