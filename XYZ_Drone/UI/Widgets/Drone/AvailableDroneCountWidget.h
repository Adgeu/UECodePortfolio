// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AvailableDroneCountWidget.generated.h"

class UTextBlock;

UCLASS()
class XYZHOMEWORK_API UAvailableDroneCountWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AvailableCountText;

	virtual void NativeConstruct() override;

private:
	void UpdateDroneCount(int32 NewCount);
};
