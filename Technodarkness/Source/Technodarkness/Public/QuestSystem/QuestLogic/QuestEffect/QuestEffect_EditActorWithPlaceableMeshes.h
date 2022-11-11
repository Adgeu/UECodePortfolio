// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestEffects/QuestEffect.h"
#include "QuestEffect_EditActorWithPlaceableMeshes.generated.h"

///////////////////////////////////////////////////////////////////

/** Sign of operation that we want to do with actor. */
UENUM()
enum class EEditActorWithPlaceableMeshesOperationSign : uint8
{
	// We want to add static meshes.
	AddStaticMeshes,
	// We want to remove static meshes.
	RemoveStaticMeshes
};

///////////////////////////////////////////////////////////////////

/** Operation that we want to do with actor. */
USTRUCT(BlueprintType)
struct FEditActorWithPlaceableMeshesOperation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EEditActorWithPlaceableMeshesOperationSign OperationSign = EEditActorWithPlaceableMeshesOperationSign::AddStaticMeshes;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> StaticMeshes;
};

///////////////////////////////////////////////////////////////////

/**
 * Edits actor with placeable meshes.
 */
UCLASS()
class TECHNODARKNESS_API UQuestEffect_EditActorWithPlaceableMeshes : public UQuestEffect
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;

protected:
	/**
	 * Id of the actor that we want to edit. Actor must be AActorWithPlaceableMeshes.
	 * @see AActorWithPlaceableMeshes
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Settings")
	FGameplayTag ActorTargetId;

	// Operations for edit the actor.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Settings", meta = (TitleProperty = "OperationSign"))
	TArray<FEditActorWithPlaceableMeshesOperation> Operations;
};
