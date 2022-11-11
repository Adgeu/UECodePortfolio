// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)


#include "Characters/TDPlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "CraftingSystem/Components/CraftingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/QuestLogComponent.h"
#include "UI/Widgets/Crafting/CraftingWidget.h"

ATDPlayerCharacter::ATDPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QuestLogNewComponent = CreateDefaultSubobject<UQuestLogComponent>("QuestLogComponent");
	CraftingComponent = CreateDefaultSubobject<UCraftingComponent>("CraftingComponent");
}

void ATDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CraftingComponent->OnEndEnterCraftingMode.AddDynamic(this, &ATDPlayerCharacter::OnBeginEnterCraftingMode);
	CraftingComponent->OnEndEnterCraftingMode.AddDynamic(this, &ATDPlayerCharacter::OnEndEnterCraftingMode);
	CraftingComponent->OnBeginExitCraftingMode.AddDynamic(this, &ATDPlayerCharacter::OnBeginExitCraftingMode);
}

void ATDPlayerCharacter::OnBeginEnterCraftingMode()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void ATDPlayerCharacter::OnEndEnterCraftingMode()
{
	if (!CraftingWidgetClass)
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Crafting widget class is not set!"), *FString(__FUNCTION__));
		return;
	}
	
	CraftingWidget = CreateWidget<UCraftingWidget>(GetWorld(), CraftingWidgetClass);
	CraftingWidget->SetCraftingComponent(CraftingComponent);
	CraftingWidget->RefreshRecipesList();
	CraftingWidget->AddToViewport();

	APlayerController* PlayerController = GetController<APlayerController>();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
}

void ATDPlayerCharacter::OnBeginExitCraftingMode()
{
	if (CraftingWidget)
	{
		CraftingWidget->RemoveFromParent();
		CraftingWidget->Destruct();

		APlayerController* PlayerController = GetController<APlayerController>();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}
