#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"

class FQuestSystemEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    FORCEINLINE EAssetTypeCategories::Type GetQuestCategoryBit() const
    {
        return QuestCategoryBit;
    }

private:
    void RegisterQuestAsset();
    void UnregisterQuestAsset();
    
    TSharedPtr<class FAssetTypeActions_Quest> QuestTypeActions;
    TSharedPtr<struct FGraphPanelNodeFactory> QuestSchemaGraphPanelNodeFactory;
    TSharedPtr<struct FGraphPanelPinFactory> QuestSchemaPanelPinFactory;
    TSharedPtr<struct FGraphPanelPinConnectionFactory> QuestSchemaPanelPinConnectionFactory;

    EAssetTypeCategories::Type QuestCategoryBit;
};
