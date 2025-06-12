// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "Components/DreamSequenceSubtitleWidget.h"

#include "DreamSequenceSubtitleLog.h"
#include "Components/DreamSequenceSubtitleEntryWidget.h"
#include "DreamSequenceSubtitleSettings.h"
#include "DreamSequenceSubtitleWorldSubsystem.h"
#include "Animation/UMGSequencePlayer.h"
#include "Misc/EngineVersionComparison.h"

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
	if (UDreamSequenceSubtitleSettings::Get()->bSubtitleWidgetEntryAnimation)
	{
#if UE_VERSION_NEWER_THAN_OR_EQUAL(5,6,0)
		PlayAnimationForward(Animation_In).GetAnimationState()->GetOnWidgetAnimationFinished().AddLambda([this](FWidgetAnimationState& State)
		{
			BP_Activated();
		});
#else
		PlayAnimationForward(Animation_In)->OnSequenceFinishedPlaying().AddLambda([this](const UUMGSequencePlayer& Player)
		{
			BP_Activated();
		});
#endif
	}
	else
	{
		BP_Activated();
	}
}

void UDreamSequenceSubtitleWidget::Deactivated()
{
	if (UDreamSequenceSubtitleSettings::Get()->bSubtitleWidgetEntryAnimation)
	{
#if UE_VERSION_NEWER_THAN_OR_EQUAL(5,6,0)
		PlayAnimationForward(Animation_Out).GetAnimationState()->GetOnWidgetAnimationFinished().AddLambda([this](FWidgetAnimationState& State)
		{
			BP_Deactivated();
		});
#else
		PlayAnimationForward(Animation_Out)->OnSequenceFinishedPlaying().AddLambda([this](const UUMGSequencePlayer& Player)
		{
			BP_Deactivated();
		});
#endif
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
