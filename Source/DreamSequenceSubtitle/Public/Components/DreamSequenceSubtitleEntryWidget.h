// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamSequenceSubtitleTypes.h"
#include "Blueprint/UserWidget.h"
#include "DreamSequenceSubtitleEntryWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DREAMSEQUENCESUBTITLE_API UDreamSequenceSubtitleEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FDreamSequenceSubtitleSectionContent Content;

public:
	bool SetContent(const FDreamSequenceSubtitleSectionContent& Entry);

public:
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Set Dialogue Content")
	void BP_SetDialogueContent(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Set Speaker Name")
	void BP_SetDialogueSpeakerName(const FText& InText);

	virtual void Activated();
	virtual void Deactivated();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Activate")
	void BP_Activated();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Deactivate")
	void BP_Deactivated();

public:
	UPROPERTY(BlueprintReadOnly, Transient, Meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_In;

	UPROPERTY(BlueprintReadOnly, Transient, Meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_Out;
};
