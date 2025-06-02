// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "Components/DreamSequenceSubtitleWidget.h"

#include "DreamSequenceSubtitleLog.h"
#include "Components/DreamSequenceSubtitleEntryWidget.h"
#include "DreamSequenceSubtitleSettings.h"
#include "DreamSequenceSubtitleWorldSubsystem.h"
#include "Animation/UMGSequencePlayer.h"
#include "Editor/WidgetCompilerLog.h"

void UDreamSequenceSubtitleWidget::RemoveEntryByWidget(UDreamSequenceSubtitleEntryWidget* EntryWidget)
{
	if (EntriesWidgetsMap.Contains(EntryWidget))
	{
		EntriesWidgetsMap.Remove(EntryWidget);
		EntryWidget->Deactivated();
	}
}

UDreamSequenceSubtitleEntryWidget* UDreamSequenceSubtitleWidget::AddEntry(FDreamSequenceSubtitleSectionContent Entry)
{
	TSubclassOf<UDreamSequenceSubtitleEntryWidget> EntryWidgetClass = UDreamSequenceSubtitleSettings::Get()->GetSubtitleEntryWidgetClass();

	if (!EntryWidgetClass)
	{
		return nullptr;
	}

	UDreamSequenceSubtitleEntryWidget* EntryWidget = CreateWidget<UDreamSequenceSubtitleEntryWidget>(this, EntryWidgetClass);

	if (EntryWidget)
	{
		EntryWidget->SetContent(Entry);
		AddEntryWidgetTo(EntryWidget);
		EntryWidget->Activated();

		EntriesWidgetsMap.Add(EntryWidget, Entry);
		OnEntryChanged.Broadcast(Entry, EntryWidget);
	}
	else
	{
		DSS_FLOG(Error, "EntryWidget is nullptr");
	}

	return EntryWidget;
}

void UDreamSequenceSubtitleWidget::Activated()
{
	if (UDreamSequenceSubtitleSettings::Get()->bSubtitleWidgetAnimation)
	{
		PlayAnimationForward(Animation_In)->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& Player)
		{
		});
		BP_Activated();
	}
	else
	{
		BP_Activated();
	}
}

void UDreamSequenceSubtitleWidget::Deactivated()
{
	if (UDreamSequenceSubtitleSettings::Get()->bSubtitleWidgetAnimation)
	{
		PlayAnimationForward(Animation_Out)->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer& Player)
		{
			BP_Deactivated();
		});
	}
	else
	{
		BP_Deactivated();
	}
}

void UDreamSequenceSubtitleWidget::BP_Activated_Implementation()
{
}

void UDreamSequenceSubtitleWidget::BP_Deactivated_Implementation()
{
	UDreamSequenceSubtitleWorldSubsystem::Get(GWorld)->RemoveWidget();
}
