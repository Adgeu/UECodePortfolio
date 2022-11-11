// TechnoDarkness
// Code authors: Alexey Faradjev (myndbyrd)

#pragma once

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

// Utils for working with project assets.
namespace AssetUtils
{
	/**
	 * Returns all assets of class.
	 * 
	 * @tparam TAssetClass Class of the asset.
	 * @param bRecursiveClasses If true, subclasses will also be included and RecursiveClassesExclusionSet will be excluded.
	 * @return Array of the found assets.
	 */
	template <typename TAssetClass>
	TArray<TAssetClass*> GetAssetsOfClass(bool bRecursiveClasses = true)
	{
		const UClass* Base = TAssetClass::StaticClass();
		const IAssetRegistry* AssetRegistry = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	
		FARFilter Filter;
		Filter.ClassNames.Add(Base->GetFName());
		Filter.bRecursiveClasses = bRecursiveClasses;

		TArray<FAssetData> AssetDataArr;
		AssetRegistry->GetAssets(Filter, AssetDataArr);

		TArray<TAssetClass*> Result;
		Result.Reserve(AssetDataArr.Num());
		
		for (const FAssetData& AssetData : AssetDataArr)
		{
			Result.Emplace(StaticCast<TAssetClass*>(AssetData.GetAsset()));
		}

		return Result;
	}

	/**
	 * Returns all assets of class in specific directory.
	 * If Dir param is empty, returns all assets in project.
	 * 
	 * @tparam TAssetClass Class of the asset.
	 * @param Dir Directory from which we want to get assets (including subdirectories).
	 * @param bRecursiveClasses If true, subclasses will also be included and RecursiveClassesExclusionSet will be excluded.
	 * @return Array of the found assets.
	 */
	template <typename TAssetClass>
	TArray<TAssetClass*> GetAssetsOfClassInDirectory(const FString& Dir, bool bRecursiveClasses = true)
	{
		return GetAssetsOfClass<TAssetClass>(bRecursiveClasses).FilterByPredicate([Dir](const TAssetClass* Asset)
		{
			return Dir.IsEmpty() || Asset->GetFullName().Contains(Dir);
		});
	}
}
