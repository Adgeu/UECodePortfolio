// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#include "Actors/Interactive/Radio.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Speech/SpeechComponent.h"
#include "Technodarkness/TDTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogRadio, All, All);

ARadio::ARadio()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(GetRootComponent());
	
	OverlappingVolume = CreateDefaultSubobject<USphereComponent>("OverlappingVolume");
	OverlappingVolume->SetupAttachment(GetRootComponent());
	OverlappingVolume->SetCollisionProfileName(CollisionProfileTrigger);
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(GetRootComponent());

	SpeechComponent = CreateDefaultSubobject<USpeechComponent>("SpeechComponent");
}

void ARadio::BeginPlay()
{
	Super::BeginPlay();
	
	AudioComponent->OnAudioFinished.AddDynamic(this, &ARadio::OnSoundFinished);
	SpeechComponent->OnSpeechAssetStart.AddDynamic(this, &ARadio::OnSpeechAssetBegin);
	SpeechComponent->OnSpeechAssetEnd.AddDynamic(this, &ARadio::OnSpeechAssetEnd);
	SpeechComponent->OnSpeechPhraseStart.AddDynamic(this, &ARadio::OnSpeechPhraseBegin);
}

void ARadio::OnInteract_Implementation(AActor* WithInstigator)
{
	Super::OnInteract_Implementation(WithInstigator);

	AudioComponent->Stop();
	
	if (!InteractBeginSound.IsNull())
	{
		PlaySound(InteractBeginSound.LoadSynchronous());

		FTimerHandle DelayTimer;
		FTimerDelegate Delegate;
		Delegate.BindUObject(SpeechComponent, &USpeechComponent::PlaySpeechAssetWithTag, SpeechTag, 0.0f);
		GetWorld()->GetTimerManager().SetTimer(DelayTimer, Delegate, InteractBeginSound->GetDuration(), false);
	}
	else
	{
		SpeechComponent->PlaySpeechAssetWithTag(SpeechTag);
	}
}

void ARadio::OnOverlapVolume()
{
	if (CurrentRadioState != ERadioState::Inactive)
	{
		return;
	}

	SetNewRadioState(ERadioState::PlayingMusic);
	PlayNextMusicSound(FirstMusicStartTimePercent);
}

void ARadio::PlaySound(USoundBase* Sound, float StartTimePercent)
{
	if (Sound)
	{
		AudioComponent->SetSound(Sound);
		
		float StartTime = 0.0f;
		if (StartTimePercent != 0.0f)
		{
			StartTimePercent = FMath::Clamp(StartTimePercent, 0.0f, 1.0f);
			if (StartTimePercent == 1.0f)
			{
				OnSoundFinished();
				return;
			}
			
			const FVector2D InputRange(0.0f, 1.0f);
			const FVector2D OutputRange(0.0f, Sound->GetDuration());
			
			StartTime = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, StartTimePercent);
		}
		
		AudioComponent->Play(StartTime);
	}
	else
	{
		UE_LOG(LogRadio, Warning, TEXT("%s() Trying to play sound asset that does not set!"), *FString(__FUNCTION__));
	}
}

void ARadio::OnDeactivationEnd()
{
	OverlappingVolume->DestroyComponent();
	AudioComponent->DestroyComponent();
}

void ARadio::PlayNextMusicSound(float StartTimePercent)
{
	if (++CurrentMusicIndex < MusicSounds.Num())
	{
		PlaySound(MusicSounds[CurrentMusicIndex].LoadSynchronous(), StartTimePercent);
	}
	else
	{
		PlaySound(WhiteNoiseSound.LoadSynchronous(), StartTimePercent);
	}
}

void ARadio::SetNewRadioState(ERadioState NewRadioState)
{
	CurrentRadioState = NewRadioState;
	OnRadioStateChanged.Broadcast(CurrentRadioState);
}

void ARadio::OnSoundFinished()
{
	if (CurrentRadioState == ERadioState::PlayingMusic)
	{
		PlayNextMusicSound();
	}
	else if (CurrentRadioState == ERadioState::Deactivated)
	{
		OnDeactivationEnd();
	}
}

void ARadio::OnSpeechAssetBegin(const USpeechAsset* SpeechAsset)
{
	SetNewRadioState(ERadioState::PlayingSpeech);
}

void ARadio::OnSpeechAssetEnd(const USpeechAsset* SpeechAsset)
{
	SetNewRadioState(ERadioState::Deactivated);
	
	if (!DeactivateSound.IsNull())
	{
		PlaySound(DeactivateSound.LoadSynchronous());
	}
	else
	{
		OnDeactivationEnd();
	}
}

void ARadio::OnSpeechPhraseBegin(const FSpeechPhrase& SpeechPhrase)
{
	if (!SpeechPhrase.Sound.IsNull())
	{
		PlaySound(SpeechPhrase.Sound.LoadSynchronous());
	}
}
