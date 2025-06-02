using UnrealBuildTool;

public class DreamSequenceSubtitleEditor : ModuleRules
{
    public DreamSequenceSubtitleEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "Sequencer",
                "DreamSequenceSubtitle",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Sequencer",
                "EditorWidgets",
                "SequenceDialogue",
                "EditorStyle",
                "UnrealEd",
                "MovieScene",
                "Projects",
            }
        );
    }
}