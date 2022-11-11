// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SpeechAssetFactory.generated.h"

UCLASS()
class TECHNODARKNESSEDITOR_API USpeechAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USpeechAssetFactory();
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

