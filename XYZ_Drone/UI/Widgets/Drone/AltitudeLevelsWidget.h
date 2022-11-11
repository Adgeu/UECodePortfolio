// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AltitudeLevelsWidget.generated.h"

class UImage;

class APlayerDrone;

UCLASS()
class XYZHOMEWORK_API UAltitudeLevelsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateDroneBindings();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* AltitudeLevelsImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	FName AltitudeParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials", meta = (ClampMin = 0.0, UIMin = 0.0))
	float AltitudeParamMultiplier = 0.001;

	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* AltitudeLevelsMaterial;
	
	void UpdateAltitude(bool bResult, float Value);

	FDelegateHandle AltitudeDelegateHandle;

	void ClearDelegateBindings(APlayerDrone* DroneFor);
	void SetDelegateBindings(APlayerDrone* DroneFor);
};
