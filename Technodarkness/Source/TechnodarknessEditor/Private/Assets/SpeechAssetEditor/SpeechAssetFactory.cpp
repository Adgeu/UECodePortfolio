// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/SpeechAssetEditor/SpeechAssetFactory.h"

#include "Speech/SpeechAsset.h"

USpeechAssetFactory::USpeechAssetFactory()
{
	SupportedClass = USpeechAsset::StaticClass();
	bCreateNew = true;
}

UObject* USpeechAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<USpeechAsset>(InParent, Class, Name, Flags, Context);
}

