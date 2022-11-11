using UnrealBuildTool;

public class QuestSystemEditor : ModuleRules
{
    public QuestSystemEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "AssetTools"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Kismet",
                "PropertyEditor",
                "UnrealEd",
                "EditorStyle",
                "GraphEditor",
                "Projects",
                "BlueprintGraph",
                "KismetWidgets",
                "ToolMenus",
                "ApplicationCore",
                "QuestSystem", 
            }
        );
        
        PrivateIncludePaths.AddRange(
            new string[]
            {
                "QuestSystemEditor"
            }
        );
    }
}