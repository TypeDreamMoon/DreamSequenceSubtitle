// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "Components/DreamSequenceSubtitleEntryWidget.h"

#include "Misc/EngineVersionComparison.h"

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

void UDreamSequenceSubtitleEntryWidget::Deactivated()
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

void UDreamSequenceSubtitleEntryWidget::BP_Deactivated_Implementation()
{
}

void UDreamSequenceSubtitleEntryWidget::BP_Activated_Implementation()
{
}
