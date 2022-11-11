// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "QuestLogic/QuestLogicBase.h"
#include "QuestTrigger.generated.h"

/**
 * Quest trigger can track various states.
 * When the condition of the trigger met, the trigger entering in the triggered state.
 * If the trigger in the triggered state, we can undo it. Then the trigger waits the condition met for entering in triggered state again.
 *
 * - Use OnTrackingBegins and OnTrackingEnded for setting state tracking and then clear it.
 * - For entering in the triggered state call EnterTriggeredState.
 * - For undo triggered state call UndoTriggeredState. We can undo trigger only if bCanUndo is TRUE.
 */
UCLASS(Abstract, Blueprintable)
class QUESTSYSTEM_API UQuestTrigger : public UQuestLogicBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnTriggeredStateChangedSignature, const UQuestTrigger* /*InTrigger*/, bool /*bNewTriggeredState*/);

	FOnTriggeredStateChangedSignature OnTriggeredStateChanged;
	
public:
	UQuestTrigger();

	virtual FString GetDescription(int32 IndentLevel = 0) const override;
	
	/** Begins trigger tracking. */
	void BeginTracking();

	/** Ends trigger tracking. */
	void EndTracking();

	// FTickableGameObject interface
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	// ~FTickableGameObject interface

	/** Undo triggered state. */
	UFUNCTION(BlueprintCallable)
	void UndoTriggeredState();

	/** Is trigger in triggered state. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsTriggered() const
	{
		return bInvertTriggeredState ? !bIsTriggered : bIsTriggered;
	}

	/** Is trigger in tracking state. */
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsTracking() const
	{
		return bIsTracking;
	}
	
protected:
	/** Tracking was begins. */
	UFUNCTION(BlueprintNativeEvent)
	void OnTrackingBegins();

	/** Tracking was ended. */
	UFUNCTION(BlueprintNativeEvent)
	void OnTrackingEnded();

	UFUNCTION(BlueprintNativeEvent)
	void OnUndoTriggeredState();
	
	/** Notifies that conditions of the trigger was met and we should enter in triggered state. */
	UFUNCTION(BlueprintCallable)
	void EnterTriggeredState();

	/** 
	 *	Function called every frame on this trigger. Override this function to implement custom logic to be executed every frame.
	 *	Note that Tick is disabled by default, and you will need to override IsTriggerTickable and set return value to true to enable it.
	 *
	 *	@param DeltaSeconds	Game time elapsed during last frame modified by the time dilation
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void ReceiveTick(float DeltaSeconds);

	// Enable tick function.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Settings")
	uint8 bTickableTrigger : 1;

	// Can undo triggered state.
	// If TRUE we give permission to this trigger or outer systems to undo triggered state.
	// If FALSE the trigger only can enter in triggered state but can't undo it.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Settings")
	uint8 bCanUndo : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Settings")
	uint8 bInvertTriggeredState : 1;

private:
	// Is trigger in triggered state.
	uint8 bIsTriggered : 1;

	// Is trigger in tracking state.
	uint8 bIsTracking : 1;
};
