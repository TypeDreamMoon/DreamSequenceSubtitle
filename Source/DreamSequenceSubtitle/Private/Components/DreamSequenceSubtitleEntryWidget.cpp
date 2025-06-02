// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "Components/DreamSequenceSubtitleEntryWidget.h"

#include "DreamSequenceSubtitleSettings.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Editor/WidgetCompilerLog.h"


bool UDreamSequenceSubtitleEntryWidget::SetContent(const FDreamSequenceSubtitleSectionContent& Entry)
{
	BP_SetDialogueSpeakerName(Entry.Speaker);
	BP_SetDialogueContent(Entry.Content);
	return true;
}

void UDreamSequenceSubtitleEntryWidget::Activated()
{
	if (UDreamSequenceSubtitleSettings::Get()->bSubtitleWidgetEntryAnimation)
	{
		PlayAnimationForward(Animation_In)->OnSequenceFinishedPlaying().AddLambda([this](const UUMGSequencePlayer& Player)
		{
		});
	}
	else
	{
		BP_Activated();
	}
}

void UDreamSequenceSubtitleEntryWidget::Deactivated()
{
	if (UDreamSequenceSubtitleSettings::Get()->bSubtitleWidgetEntryAnimation)
	{
		PlayAnimationForward(Animation_Out)->OnSequenceFinishedPlaying().AddLambda([this](const UUMGSequencePlayer& Player)
		{
			BP_Deactivated();
		});
	}
	else
	{
		BP_Activated();
	}
}

void UDreamSequenceSubtitleEntryWidget::BP_Deactivated_Implementation()
{
}

void UDreamSequenceSubtitleEntryWidget::BP_Activated_Implementation()
{
}
