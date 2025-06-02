#include "DreamSequenceSubtitleEditorModule.h"

#include "DreamSequenceSubtitleEditor.h"
#include "DreamSequenceSubtitleEditorStyle.h"
#include "ISequencerModule.h"

#define LOCTEXT_NAMESPACE "FDreamSequenceSubtitleEditorModule"

void FDreamSequenceSubtitleEditorModule::StartupModule()
{
    FDreamSequenceSubtitleStyle::Initialize();
    ISequencerModule& Module = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
    Handle = Module.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FDreamSequenceSubtitleEditor::CreateTrackEditor));
}

void FDreamSequenceSubtitleEditorModule::ShutdownModule()
{
    if (!FModuleManager::Get().IsModuleLoaded("Sequencer"))
    {
        return;
    }

    ISequencerModule& Module = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");

    Module.UnRegisterTrackEditor(Handle);

    FDreamSequenceSubtitleStyle::Shutdown();
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDreamSequenceSubtitleEditorModule, DreamSequenceSubtitleEditor)