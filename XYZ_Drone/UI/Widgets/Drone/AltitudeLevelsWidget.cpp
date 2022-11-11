// For educational purposes only. 


#include "UI/Widgets/Drone/AltitudeLevelsWidget.h"

#include "Components/Image.h"
#include "Drone/PlayerDrone.h"

void UAltitudeLevelsWidget::UpdateDroneBindings()
{
	APlayerDrone* Drone = GetOwningPlayerPawn<APlayerDrone>();
	if (IsValid(Drone))
	{
		ClearDelegateBindings(Drone);
		SetDelegateBindings(Drone);
	}
}

void UAltitudeLevelsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AltitudeLevelsMaterial = AltitudeLevelsImage->GetDynamicMaterial();
}

void UAltitudeLevelsWidget::UpdateAltitude(bool bResult, float Value)
{
	if (IsValid(AltitudeLevelsMaterial))
	{
		AltitudeLevelsMaterial->SetScalarParameterValue(AltitudeParamName, -Value * AltitudeParamMultiplier);
	}
}

void UAltitudeLevelsWidget::ClearDelegateBindings(APlayerDrone* DroneFor)
{
	DroneFor->OnAltitudeChanges.Remove(AltitudeDelegateHandle);
}

void UAltitudeLevelsWidget::SetDelegateBindings(APlayerDrone* DroneFor)
{
	AltitudeDelegateHandle = DroneFor->OnAltitudeChanges.AddUObject(this, &UAltitudeLevelsWidget::UpdateAltitude);
}
