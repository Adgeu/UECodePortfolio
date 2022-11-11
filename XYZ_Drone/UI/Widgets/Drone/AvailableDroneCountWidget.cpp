// For educational purposes only. 


#include "UI/Widgets/Drone/AvailableDroneCountWidget.h"

#include "Components/TextBlock.h"
#include "Characters/XHBaseCharacter.h"
#include "Components/Drone/DroneControllerComponent.h"

void UAvailableDroneCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const AXHBaseCharacter* Character = GetOwningPlayerPawn<AXHBaseCharacter>();
	if (IsValid(Character))
	{
		UDroneControllerComponent* DroneControllerComponent = Character->GetDroneControllerComponent();
		if (IsValid(DroneControllerComponent))
		{
			DroneControllerComponent->OnAvailableDroneCountChanges.AddUObject(this, &UAvailableDroneCountWidget::UpdateDroneCount);
		}
	}
}

void UAvailableDroneCountWidget::UpdateDroneCount(int32 NewCount)
{
	AvailableCountText->SetText(FText::AsNumber(NewCount));
}
