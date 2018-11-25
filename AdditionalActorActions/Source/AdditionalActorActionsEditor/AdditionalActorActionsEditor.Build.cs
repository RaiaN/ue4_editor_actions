// Copyright Peter Leontev

using UnrealBuildTool;

public class AdditionalActorActionsEditor : ModuleRules
{
	public AdditionalActorActionsEditor(ReadOnlyTargetRules rules) : base(rules)
	{
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        PublicIncludePaths.AddRange(new string[]{"AdditionalActorActionsEditor/Public"});
		PrivateIncludePaths.AddRange(new string[]{"AdditionalActorActionsEditor/Private"});

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "EditorStyle",
                "Engine",
                "KismetWidgets",
                "PostworldGame",
                "Slate",
                "SlateCore",
                "UnrealEd",
                
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "InputCore",
                "SlateCore",
                "PropertyEditor",
                "LevelEditor"
            }
        );
    }
}
