#include "QuestStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"

#define IMAGE_BRUSH(Style, RelativePath, ...) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

TSharedPtr<FSlateStyleSet> FQuestStyle::QuestStyleInstance = nullptr;

void FQuestStyle::Initialize()
{
	if (!QuestStyleInstance.IsValid())
	{
		QuestStyleInstance = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
		const FString ContentDir = IPluginManager::Get().FindPlugin("QuestSystemPlugin")->GetBaseDir();
		QuestStyleInstance->SetContentRoot(ContentDir);

		const FVector2D Icon16x16(16.0f, 16.0f);
		const FVector2D Icon32x32(32.0f, 32.0f);
		const FVector2D Icon64x64(64.0f, 64.0f);
		const FVector2D Icon128x128(128.0f, 128.0f);

		QuestStyleInstance->Set("ClassThumbnail.Quest", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/QuestIcon_128px"), Icon128x128));
		QuestStyleInstance->Set("ClassIcon.Quest", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/QuestIcon_128px"), Icon16x16));
		QuestStyleInstance->Set("QuestIcons.Check", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Check_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Weight", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Weight_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Enter", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Enter_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Exit", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Exit_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Scroll", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Scroll_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Objective", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Objective_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Effect", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Effect_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Random", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Random_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Selector", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Selector_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Sequence", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Sequence_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Wait", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Wait_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.ObjectiveXORCompletion", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/ObjectiveXORCompletion_64px"), Icon64x64));
		QuestStyleInstance->Set("QuestIcons.Hidden", new IMAGE_BRUSH(QuestStyleInstance, TEXT("Resources/Hidden_32px"), Icon32x32));

		FSlateStyleRegistry::RegisterSlateStyle(*QuestStyleInstance);
	}
}

void FQuestStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*QuestStyleInstance);
	ensure(QuestStyleInstance.IsUnique()); 
	QuestStyleInstance.Reset();
}

const ISlateStyle& FQuestStyle::Get()
{
	return *QuestStyleInstance;
}

FName FQuestStyle::GetStyleSetName()
{
	return TEXT("QuestStyle");
}

#undef IMAGE_BRUSH
