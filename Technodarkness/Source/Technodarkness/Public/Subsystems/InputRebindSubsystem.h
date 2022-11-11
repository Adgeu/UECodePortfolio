// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputRebindSubsystem.generated.h"

struct FInputAxis;

/**
 * Provides input rebind functionality.
 */
UCLASS()
class TECHNODARKNESS_API UInputRebindSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * Rebind key in actions.
	 * @param InActions Actions that we want to be rebinded.
	 * @param OldKey Old key that was already binded.
	 * @param NewKey New key that we want to bind to actions.
	 */
	void RebindKeyInActions(const TArray<FName>& InActions, const FKey& OldKey, const FKey& NewKey);

	/**
	 * Rebind key in axes.
	 * @param InAxes Axes that we want to be rebinded.
	 * @param OldKey Old key that was already binded.
	 * @param NewKey New key that we want to bind to axes.
	 */
	void RebindKeyInAxes(const TArray<FInputAxis>& InAxes, const FKey& OldKey, const FKey& NewKey);

	/**
	 * Gets all displaying input names that binded to the key. 
	 * @param Key We searching inputs with this key.
	 * @param InputDisplayNames [OUT] Searching result.
	 * @return Have we found any inputs with the given key.
	 */
	bool GetInputDisplayNamesByKey(const FKey& Key, OUT TArray<FText>& InputDisplayNames) const;

	/**
	 * Gets displaying name text of the action.
	 * @param ActionName Action that we want to find.
	 * @param InputDisplayName Displaying name text of the given action.
	 * @return Have we found displaying name text with the given action.
	 */
	bool GetInputDisplayNameByActionName(const FName& ActionName, OUT FText& InputDisplayName) const;

	/**
	 * Gets displaying name text of the axis.
	 * @param Axis Axis that we want to find.
	 * @param InputDisplayName Displaying name text of the given axis.
	 * @return Have we found displaying name text with the given axis.
	 */
	bool GetInputDisplayNameByInputAxis(const FInputAxis& Axis, OUT FText& InputDisplayName) const;

	/**
	 * Is the key has any bindings.
	 * @note Searching in actions and axes from the InputRebindSettings.
	 * @see UInputRebindSettings
	 */
	bool IsKeyHasAnyBindings(const FKey& Key) const;

	/** Gets keys that binded to the action. */
	void GetActionKeys(const FName& InAction, OUT FKey& FirstSlotKey, OUT FKey& SecondSlotKey) const;

	/** Gets keys that binded to the axis with the scale. */
	void GetAxisKeys(const FName& InAxis, const float Scale, OUT FKey& FirstSlotKey, OUT FKey& SecondSlotKey) const;
};
