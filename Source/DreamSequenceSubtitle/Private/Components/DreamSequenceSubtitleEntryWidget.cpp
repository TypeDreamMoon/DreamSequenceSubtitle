// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "Components/DreamSequenceSubtitleEntryWidget.h"

void UDreamSequenceSubtitleEntryWidget::Deactivate_Implementation()
{
	RemoveFromParent();
}

bool UDreamSequenceSubtitleEntryWidget::SetContent(const FDreamSequenceSubtitleSectionContent& Entry)
{
	BP_SetDialogueSpeakerName(Entry.Speaker);
	BP_SetDialogueContent(Entry.Content);
	return true;
}
