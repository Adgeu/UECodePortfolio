// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroneWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

class APlayerDrone;

UCLASS()
class XYZHOMEWORK_API UDroneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UAltitudeLevelsWidget* GetAltitudeLevelsWidget() const;

	void UpdateDroneBindings();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* HorizonIndicatorImage;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* AccelerationBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* SpeedBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthAttentionText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AltitudeValueText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceValueText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float ProgressBarInterpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget names")
	FName AltitudeLevelsWidgetName;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	float TargetAccelerationPercent = 0;
	float TargetSpeedPercent = 0;

	void InterpAccelerationPercent(float DeltaSeconds);
	void InterpSpeedPercent(float DeltaSeconds);
	
	void UpdateRollAngle(float Angle);
	void UpdateAcceleration(float Value);
	void UpdateSpeed(float Value, float Percent);
	void UpdateHealth(float Value, float Percent);
	void UpdateAltitude(bool bResult, float Value);
	void UpdateDistance(bool bResult, float Value);

	FDelegateHandle AccelerationDelegateHandle;
	FDelegateHandle SpeedDelegateHandle;
	FDelegateHandle RollAngleDelegateHandle;
	FDelegateHandle HealthDelegateHandle;
	FDelegateHandle AltitudeDelegateHandle;
	FDelegateHandle DistanceDelegateHandle;

	void ClearDelegateBindings(APlayerDrone* DroneFor);
	void SetDelegateBindings(APlayerDrone* DroneFor);
};
