// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractableActor.h"
#include "Speech/SpeechInterface.h"
#include "Speech/SpeechTypes.h"
#include "Radio.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USpeechComponent;
class UAudioComponent;
class USoundBase;
class USoundAttenuation;

////////////////////////////////////////////////////////////////////

/**
 * Radio states.
 */
UENUM(BlueprintType)
enum class ERadioState : uint8
{
	// Radio inactive and waits for the character overlapping with OverlappingVolume.
	Inactive,

	// Radio playing music.
	PlayingMusic,

	// Radio playing speech.
	PlayingSpeech,

	// Radio deactivated.
	Deactivated
};

////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioStateChanged, ERadioState, RadioState);

////////////////////////////////////////////////////////////////////

class USpeechAsset;

/**
 * Interactive radio actor.
 * When character begins overlap volume, music stats playing.
 * Speech starts playing on interact.
 * @note Radio actor handles playing speech phrase sound.
 */
UCLASS(Abstract)
class TECHNODARKNESS_API ARadio : public AInteractableActor, public ISpeechInterface
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	ARadio();

	// Radio state was changed.
	UPROPERTY(BlueprintAssignable)
	FOnRadioStateChanged OnRadioStateChanged;

protected:
	virtual void BeginPlay() override;

	/** Interacting with radio. */
	virtual void OnInteract_Implementation(AActor* WithInstigator) override;

	/** Main character overlapping with volume. */
	UFUNCTION(BlueprintCallable)
	void OnOverlapVolume();

	/**
	 * Play sound asset.
	 * @param Sound Sound asset that we want to play.
	 * @param StartTimePercent Sound playing start time percent in range [0, 1].
	 */
	void PlaySound(USoundBase* Sound, float StartTimePercent = 0.0f);

	// Start time percent for first music in MusicSounds array.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Settings", meta = (ClampMin = 0.0, ClampMax = 1.0))
	float FirstMusicStartTimePercent = 0.0f;

	// Music sound assets.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Settings")
	TArray<TSoftObjectPtr<USoundBase>> MusicSounds;
	
	// White noise sound asset.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Settings")
	TSoftObjectPtr<USoundBase> WhiteNoiseSound;

	// Sound asset for interaction begin.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Settings")
	TSoftObjectPtr<USoundBase> InteractBeginSound;

	// Radio deactivation sound asset.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Settings")
	TSoftObjectPtr<USoundBase> DeactivateSound;

	// Speech asset that we want to play on interaction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speech Settings")
	FGameplayTag SpeechTag;

private:
	/** Calls when radio deactivation was completed. */
	void OnDeactivationEnd();

	/**
	 * Play next music sound or white noise if there is no available music sound.
	 * @param StartTimePercent Sound playing start time percent in range [0, 1].
	 */
	void PlayNextMusicSound(float StartTimePercent = 0.0f);

	/** Set new radio state. */
	void SetNewRadioState(ERadioState NewRadioState);

	/** On sound finished. */
	UFUNCTION()
	void OnSoundFinished();

	/** On speech asset begin playing. */
	UFUNCTION()
	void OnSpeechAssetBegin(const USpeechAsset* SpeechAsset);

	/** On speech asset end playing. */
	UFUNCTION()
	void OnSpeechAssetEnd(const USpeechAsset* SpeechAsset);

	/** On speech phrase begin playing. */
	UFUNCTION()
	void OnSpeechPhraseBegin(const FSpeechPhrase& SpeechPhrase);

	// Current music index.
	int32 CurrentMusicIndex = -1;

	// Current radio state.
	ERadioState CurrentRadioState = ERadioState::Inactive;
	
	////////////////////////////////////////////////////////////////////

#pragma region Components

public:
	/** Returns radio static mesh subobject. */
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return MeshComponent;
	}

	/** Returns radio overlapping volume subobject. */
	FORCEINLINE USphereComponent* GetInteractionVolume() const
	{
		return OverlappingVolume;
	}

	/** Returns radio speech subobject. */
	FORCEINLINE virtual USpeechComponent* GetSpeechComponent_Implementation() const override
	{
		return SpeechComponent;
	}

	/** Returns radio audio subobject. */
	FORCEINLINE UAudioComponent* GetAudioComponent() const
	{
		return AudioComponent;
	}
	
private:
	// Radio mesh.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	// Overlapping volume. When character begins overlap with the volume, music starts playing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* OverlappingVolume;

	// Speech component. When character interacting with the radio, speech starts playing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpeechComponent* SpeechComponent;

	// Audio component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

#pragma endregion ~Components
};
