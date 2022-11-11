

#include "Types/QuestTypes.h"

bool FLoreContainer::HasLocalizationKey(const FString& Key) const
{
	return LoreItems.ContainsByPredicate([Key](const FLoreItem& LoreItem)
	{
		return LoreItem.LocalizationKey == Key;
	});
}

const FLoreItem& FLoreContainer::FindItemByLocalizationKey(const FString& Key) const
{
	const FLoreItem* Found = LoreItems.FindByPredicate([Key](const FLoreItem& LoreItem)
	{
		return LoreItem.LocalizationKey == Key;
	});
	checkf(Found, TEXT("%s() Localiztion key \"%s\" not found in lore items!"), *FString(__FUNCTION__), *Key);
	return *Found;
}

TArray<FLoreDisplayingElement> FLoreContainer::GetDisplayingElements(const FString& LocalizationKey) const
{
	FLoreItem LoreItem = FindItemByLocalizationKey(LocalizationKey);
	return LoreItem.DisplayingElements;
}

FLoreDisplayingElement FLoreContainer::GetDisplayingElement(const FString& LocalizationKey, int32 ElementIndex) const
{
	FLoreItem LoreItem = FindItemByLocalizationKey(LocalizationKey);
	return LoreItem.DisplayingElements[ElementIndex];
}
