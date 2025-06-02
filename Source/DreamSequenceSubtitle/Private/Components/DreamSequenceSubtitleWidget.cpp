// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "Components/DreamSequenceSubtitleWidget.h"

#include "DreamSequenceSubtitleLog.h"
#include "Components/DreamSequenceSubtitleEntryWidget.h"
#include "DreamSequenceSubtitleSettings.h"

void UDreamSequenceSubtitleWidget::RemoveEntryByWidget(UDreamSequenceSubtitleEntryWidget* EntryWidget)
{
	if (EntriesWidgetsMap.Contains(EntryWidget))
	{
		EntriesWidgetsMap.Remove(EntryWidget);
		EntryWidget->Deactivate();
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
		EntriesWidgetsMap.Add(EntryWidget, Entry);

		OnEntryChanged.Broadcast(Entry, EntryWidget);
	}
	else
	{
		DSS_FLOG(Error, "EntryWidget is nullptr");
	}

	return EntryWidget;
}
