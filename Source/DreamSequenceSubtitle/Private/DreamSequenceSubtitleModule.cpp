﻿// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamSequenceSubtitleModule.h"

#include "DreamSequenceSubtitleSettings.h"

#define LOCTEXT_NAMESPACE "FDreamSequenceSubtitleModule"

void FDreamSequenceSubtitleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UDreamSequenceSubtitleSettings::Register();
}

void FDreamSequenceSubtitleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UDreamSequenceSubtitleSettings::Unregister();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDreamSequenceSubtitleModule, DreamSequenceSubtitle)