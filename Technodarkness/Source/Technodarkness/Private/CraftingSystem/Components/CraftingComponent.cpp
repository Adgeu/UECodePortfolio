// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#include "CraftingSystem/Components/CraftingComponent.h"

#include "Characters/TDPlayerCharacter.h"
#include "CraftingSystem/Recipes/CraftingBaseRecipe.h"
#include "CraftingSystem/Recipes/CraftingInventoryItemRecipe.h"
#include "CraftingSystem/Recipes/CraftingWeaponUpgradeRecipe.h"
#if WITH_EDITORONLY_DATA
#include "Utils/AssetUtils.h"
#endif

#if WITH_EDITOR
void UCraftingComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UCraftingComponent, bRefreshRecipesArray))
	{
		if (bRefreshRecipesArray)
		{
			bRefreshRecipesArray = false;
			RefreshRecipesArray();
		}
	}
}
#endif

void UCraftingComponent::EnterCraftingMode(TScriptInterface<ICraftingWorkspace> InCraftingWorkspace)
{
	if (!CachedCharacter.IsValid())
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Cached character is not valid!"), *FString(__FUNCTION__));
		return;
	}

	if (!IsValid(InCraftingWorkspace.GetObject()))
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Crafting workspace is not valid!"), *FString(__FUNCTION__));
		return;
	}

	OnBeginEnterCraftingMode.Broadcast();
	
	CurrentCraftingWorkspace = InCraftingWorkspace;
	AvailableIngredients = CachedCharacter->GetCraftingIngredientsFromInventory();
	UpgradableWeapons = CachedCharacter->GetUpgradableWeapons();

	UAnimMontage* EnterAnimMontage = GetWorkspaceEnterAnimMontage();
	if (EnterAnimMontage)
	{
		const float MontageDuration = CachedCharacter->PlayAnimMontage(EnterAnimMontage);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCraftingComponent::EndEnterCraftingMode, MontageDuration);
	}
	else
	{
		EndEnterCraftingMode();
	}
}

void UCraftingComponent::ExitCraftingMode()
{
	if (!CachedCharacter.IsValid())
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Cached character is not valid!"), *FString(__FUNCTION__));
		return;
	}
	
	OnBeginExitCraftingMode.Broadcast();

	UAnimMontage* ExitAnimMontage = GetWorkspaceExitAnimMontage();
	if (ExitAnimMontage)
	{
		const float MontageDuration = CachedCharacter->PlayAnimMontage(ExitAnimMontage);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCraftingComponent::EndExitCraftingMode, MontageDuration);
	}
	else
	{
		EndExitCraftingMode();
	}
}

void UCraftingComponent::StartCrafting(const UCraftingBaseRecipe* Recipe)
{
	if (!CachedCharacter.IsValid())
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Cached character is not valid!"), *FString(__FUNCTION__));
		return;
	}
	
	if (!Recipe)
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Trying to craft item with null recipe!"), *FString(__FUNCTION__));
		return;
	}
	
	if (!CanCraft(Recipe))
	{
		return;
	}

	switch (Recipe->GetRecipeType())
	{
	case ERecipeType::None:
		UE_LOG(LogCrafting, Fatal, TEXT("%s() Trying to craft item by recipe with type NONE!"), *FString(__FUNCTION__));
		break;
		
	case ERecipeType::InventoryItem:
		CraftInventoryItem(StaticCast<const UCraftingInventoryItemRecipe*>(Recipe));
		break;
		
	case ERecipeType::WeaponUpgrade:
		CraftWeaponUpgrade(StaticCast<const UCraftingWeaponUpgradeRecipe*>(Recipe));
		break;
	}
}

bool UCraftingComponent::CanCraft(const UCraftingBaseRecipe* Recipe) const
{
	if (!Recipe)
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Recipe is null!"), *FString(__FUNCTION__));
		return false;
	}

	if (!CurrentCraftingWorkspace.GetObject())
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Trying check craft with null crafting workspace!"), *FString(__FUNCTION__));
		return false;
	}

	if (!EnumHasAnyFlags(Recipe->GetAllowedWorkspaces(), GetWorkspaceType()))
	{
		return false;
	}

	if (!AreThereEnoughIngredients(Recipe->GetIngredients()))
	{
		return false;
	}

	switch (Recipe->GetRecipeType())
	{
	case ERecipeType::None:
		UE_LOG(LogCrafting, Fatal, TEXT("%s() Recipe with type NONE!"), *FString(__FUNCTION__));
		return false;
		
	case ERecipeType::InventoryItem:
		return CachedCharacter->CheckFreeSpaceInInventory(Recipe->GetCraftingResultClass()->GetDefaultObject());
		
	case ERecipeType::WeaponUpgrade:
		TScriptInterface<IWeaponUpgradable> UpgradableWeapon;
		if (!GetUpgradableWeaponByClass(StaticCast<const UCraftingWeaponUpgradeRecipe*>(Recipe)->GetWeaponClass(), UpgradableWeapon))
		{
			return false;
		}
		return !IWeaponUpgradable::Execute_HasWeaponUpgrade(UpgradableWeapon.GetObject(), Recipe->GetCraftingResultClass());
	}

	return false;
}

void UCraftingComponent::AddCraftingRecipe(UCraftingBaseRecipe* RecipeToAdd)
{
	CraftingRecipes.AddUnique(RecipeToAdd);
}

TArray<UCraftingBaseRecipe*> UCraftingComponent::GetAvailableRecipes() const
{
	return CraftingRecipes.FilterByPredicate([this](const UCraftingBaseRecipe* Recipe)
	{
		return CanCraft(Recipe);
	});
}

TArray<UCraftingBaseRecipe*> UCraftingComponent::GetRecipesByWorkspaceType() const
{
	ECraftingWorkspaceType WorkspaceType = GetWorkspaceType();
	return CraftingRecipes.FilterByPredicate([WorkspaceType](const UCraftingBaseRecipe* Recipe)
	{
		return EnumHasAnyFlags(Recipe->GetAllowedWorkspaces(), WorkspaceType);
	});
}

void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ATDPlayerCharacter>(), TEXT("%s() Crafting component works only with TDPlayerCharacter!"), *FString(__FUNCTION__));
	CachedCharacter = StaticCast<ATDPlayerCharacter*>(GetOwner());
}

void UCraftingComponent::CraftInventoryItem(const UCraftingInventoryItemRecipe* Recipe)
{
	OnCraftingBegins.Broadcast(Recipe);

	UAnimMontage* CraftingAnimMontage = Recipe->GetCraftingAnimMontage();
	if (CraftingAnimMontage)
	{
		const float MontageDuration = CachedCharacter->PlayAnimMontage(CraftingAnimMontage);

		FTimerHandle TimerHandle;
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UCraftingComponent::OnCraftInventoryItemEnded, Recipe);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, MontageDuration, false);
	}
	else
	{
		OnCraftInventoryItemEnded(Recipe);
	}
}

void UCraftingComponent::OnCraftInventoryItemEnded(const UCraftingInventoryItemRecipe* Recipe)
{
	ConsumeIngredients(Recipe->GetIngredients());
	
	UObject* Item = NewObject<UObject>( GetWorld(), Recipe->GetCraftingResultClass());
	OnCraftedInventoryItem.Broadcast(Item);
	OnCraftingEnded.Broadcast(Recipe);
}

void UCraftingComponent::CraftWeaponUpgrade(const UCraftingWeaponUpgradeRecipe* Recipe)
{
	TScriptInterface<IWeaponUpgradable> UpgradableWeapon;
	if (!GetUpgradableWeaponByClass(Recipe->GetWeaponClass(), UpgradableWeapon))
	{
		return;
	}

	OnCraftingBegins.Broadcast(Recipe);

	UAnimMontage* CraftingAnimMontage = Recipe->GetCraftingAnimMontage();
	if (CraftingAnimMontage)
	{
		const float MontageDuration = CachedCharacter->PlayAnimMontage(CraftingAnimMontage);

		FTimerHandle TimerHandle;
		FTimerDelegate Delegate;
		Delegate.BindUObject(this, &UCraftingComponent::OnCraftWeaponUpgradeEnded, UpgradableWeapon.GetObject(), Recipe);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, MontageDuration, false);
	}
	else
	{
		OnCraftWeaponUpgradeEnded(UpgradableWeapon.GetObject(), Recipe);
	}
}

void UCraftingComponent::OnCraftWeaponUpgradeEnded(UObject* InWeapon, const UCraftingWeaponUpgradeRecipe* Recipe)
{
	ConsumeIngredients(Recipe->GetIngredients());
	
	IWeaponUpgradable::Execute_SetWeaponUpgrade(InWeapon, Recipe->GetCraftingResultClass());
	OnCraftedWeaponUpgrade.Broadcast(Recipe->GetWeaponClass(), Recipe->GetCraftingResultClass());
	OnCraftingEnded.Broadcast(Recipe);
}

void UCraftingComponent::EndEnterCraftingMode()
{
	OnEndEnterCraftingMode.Broadcast();
}

void UCraftingComponent::EndExitCraftingMode()
{
	OnEndExitCraftingMode.Broadcast();
	CurrentCraftingWorkspace = nullptr;
}

bool UCraftingComponent::AreThereEnoughIngredients(const TArray<FCraftingIngredient>& IngredientsToCheck) const
{
	for (const FCraftingIngredient& Ingredient : IngredientsToCheck)
	{
		const bool bContains = AvailableIngredients.ContainsByPredicate([Ingredient](const FCraftingIngredient& AvailableIngredient)
		{
			return Ingredient.IngredientClass == AvailableIngredient.IngredientClass && Ingredient.Amount <= AvailableIngredient.Amount;
		});
		
		if (!bContains)
		{
			return false;
		}
	}
	return true;
}

void UCraftingComponent::ConsumeIngredients(const TArray<FCraftingIngredient>& Ingredients)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->ConsumeCraftingIngredients(Ingredients);
		AvailableIngredients = CachedCharacter->GetCraftingIngredientsFromInventory();
	}
	else
	{
		UE_LOG(LogCrafting, Error, TEXT("%s() Cached character is not valid!"), *FString(__FUNCTION__));
	}
}

bool UCraftingComponent::GetUpgradableWeaponByClass(const TSubclassOf<UObject>& WeaponClass, TScriptInterface<IWeaponUpgradable>& OutUpgradableWeapon) const
{
	const TScriptInterface<IWeaponUpgradable>* Found = UpgradableWeapons.FindByPredicate([WeaponClass](const TScriptInterface<IWeaponUpgradable>& UpgradableWeapon)
	{
		return UpgradableWeapon.GetObject() && UpgradableWeapon.GetObject()->GetClass() == WeaponClass;
	});

	if (Found)
	{
		OutUpgradableWeapon = *Found;
		return true;
	}

	return false;
}

#if WITH_EDITORONLY_DATA
void UCraftingComponent::RefreshRecipesArray()
{
	CraftingRecipes.Empty();
	for (UCraftingBaseRecipe* RecipeAsset : AssetUtils::GetAssetsOfClassInDirectory<UCraftingBaseRecipe>(RecipesRootDir))
	{
		CraftingRecipes.Add(RecipeAsset);
	}
}
#endif
