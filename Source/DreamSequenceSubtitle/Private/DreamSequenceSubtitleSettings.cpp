// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "DreamSequenceSubtitleSettings.h"
#include "Components/DreamSequenceSubtitleWidget.h"
#include "Components/DreamSequenceSubtitleEntryWidget.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "UDreamSequenceSubtitleSettings"

UDreamSequenceSubtitleSettings::UDreamSequenceSubtitleSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDreamSequenceSubtitleSettings::Register()
{
#if WITH_EDITOR
	if (ISettingsModule* Module = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		Module->RegisterSettings("Project", "DreamPlugin", "DreamSequenceSubtitle",
		                         LOCTEXT("SettingsName", "Dream Sequence Subtitle Settings"),
		                         LOCTEXT("SettingsDescription", "Configure Dream Sequence Subtitle Settings."),
		                         Get());
	}
#endif
}

void UDreamSequenceSubtitleSettings::Unregister()
{
#if WITH_EDITOR
	if (ISettingsModule* Module = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		Module->UnregisterSettings("Project", "DreamPlugin", "DreamSequenceSubtitle");
	}
#endif
}

UDreamSequenceSubtitleSettings* UDreamSequenceSubtitleSettings::Get()
{
	return GetMutableDefault<UDreamSequenceSubtitleSettings>();
}

TSubclassOf<UDreamSequenceSubtitleWidget> UDreamSequenceSubtitleSettings::GetSubtitleWidgetClass()
{
	return SubtitleWidgetClass.LoadSynchronous();
}

TSubclassOf<UDreamSequenceSubtitleEntryWidget> UDreamSequenceSubtitleSettings::GetSubtitleEntryWidgetClass()
{
	return SubtitleEntryWidgetClass.LoadSynchronous();
}

#undef LOCTEXT_NAMESPACE