// For educational purposes only. 


#include "UI/Widgets/Drone/DroneWidget.h"

#include "AltitudeLevelsWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/CharacterComponents/HealthComponent.h"
#include "Drone/PlayerDrone.h"

UAltitudeLevelsWidget* UDroneWidget::GetAltitudeLevelsWidget() const
{
	return WidgetTree->FindWidget<UAltitudeLevelsWidget>(AltitudeLevelsWidgetName);
}

void UDroneWidget::UpdateDroneBindings()
{
	APlayerDrone* Drone = GetOwningPlayerPawn<APlayerDrone>();
	if (IsValid(Drone))
	{
		ClearDelegateBindings(Drone);
		SetDelegateBindings(Drone);
	}
	
	GetAltitudeLevelsWidget()->UpdateDroneBindings();

	Drone->UpdateDroneInfo();
}

void UDroneWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	InterpAccelerationPercent(InDeltaTime);
	InterpSpeedPercent(InDeltaTime);
}

void UDroneWidget::InterpAccelerationPercent(float DeltaSeconds)
{
	const float CurrentPercent = AccelerationBar->Percent;
	const float NewPercent = FMath::FInterpTo(CurrentPercent, TargetAccelerationPercent, DeltaSeconds, ProgressBarInterpSpeed);
	AccelerationBar->SetPercent(NewPercent);
}

void UDroneWidget::InterpSpeedPercent(float DeltaSeconds)
{
	const float CurrentPercent = SpeedBar->Percent;
	const float NewPercent = FMath::FInterpTo(CurrentPercent, TargetSpeedPercent, DeltaSeconds, ProgressBarInterpSpeed);
	SpeedBar->SetPercent(NewPercent);
}

void UDroneWidget::UpdateRollAngle(float Angle)
{
	HorizonIndicatorImage->SetRenderTransformAngle(-Angle);
}

void UDroneWidget::UpdateAcceleration(float Value)
{
	TargetAccelerationPercent = Value;
}

void UDroneWidget::UpdateSpeed(float Value, float Percent)
{
	TargetSpeedPercent = Percent;
}

void UDroneWidget::UpdateHealth(float Value, float Percent)
{
	const ESlateVisibility NewVisibility = FMath::IsNearlyEqual(Percent, 1.0f) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	HealthAttentionText->SetVisibility(NewVisibility);
	
	const int32 PercentToText = (int32)(Percent * 100.0f);
	const FText Text = FText::FromString(FString::Printf(TEXT("ATTENTION: Needs To Repair! (%d%%)"), PercentToText));
	HealthAttentionText->SetText(Text);
}

void UDroneWidget::UpdateAltitude(bool bResult, float Value)
{
	const FText Text = bResult ? FText::AsNumber((int32)Value) : FText::FromString(FString(TEXT("N/A")));
	AltitudeValueText->SetText(Text);
}

void UDroneWidget::UpdateDistance(bool bResult, float Value)
{
	const FText Text = bResult ? FText::AsNumber((int32)Value) : FText::FromString(FString(TEXT("N/A")));
	DistanceValueText->SetText(Text);
}

void UDroneWidget::ClearDelegateBindings(APlayerDrone* DroneFor)
{
	DroneFor->OnInputAccelerationChanges.Remove(AccelerationDelegateHandle);
	DroneFor->OnSpeedChanges.Remove(SpeedDelegateHandle);
	DroneFor->OnRollAngleChanges.Remove(RollAngleDelegateHandle);
	DroneFor->GetHealthComponent()->OnHealthValueChanged.Remove(HealthDelegateHandle);
	DroneFor->OnAltitudeChanges.Remove(AltitudeDelegateHandle);
	DroneFor->OnDistanceChanges.Remove(DistanceDelegateHandle);
}

void UDroneWidget::SetDelegateBindings(APlayerDrone* DroneFor)
{
	AccelerationDelegateHandle = DroneFor->OnInputAccelerationChanges.AddUObject(this, &UDroneWidget::UpdateAcceleration);
	SpeedDelegateHandle = DroneFor->OnSpeedChanges.AddUObject(this, &UDroneWidget::UpdateSpeed);
	RollAngleDelegateHandle = DroneFor->OnRollAngleChanges.AddUObject(this, &UDroneWidget::UpdateRollAngle);
	HealthDelegateHandle = DroneFor->GetHealthComponent()->OnHealthValueChanged.AddUObject(this, &UDroneWidget::UpdateHealth);
	AltitudeDelegateHandle = DroneFor->OnAltitudeChanges.AddUObject(this, &UDroneWidget::UpdateAltitude);
	DistanceDelegateHandle = DroneFor->OnDistanceChanges.AddUObject(this, &UDroneWidget::UpdateDistance);
}
