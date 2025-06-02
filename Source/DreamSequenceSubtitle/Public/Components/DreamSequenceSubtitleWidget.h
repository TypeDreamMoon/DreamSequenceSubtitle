// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamSequenceSubtitleTypes.h"
#include "Blueprint/UserWidget.h"
#include "DreamSequenceSubtitleWidget.generated.h"

class UDreamSequenceSubtitleEntryWidget;
/**
 * 
 */
UCLASS(Abstract)
class DREAMSEQUENCESUBTITLE_API UDreamSequenceSubtitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEntryChanged, const FDreamSequenceSubtitleSectionContent&, Entry, UDreamSequenceSubtitleEntryWidget*, EntryWidget);

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<UDreamSequenceSubtitleEntryWidget*, FDreamSequenceSubtitleSectionContent> EntriesWidgetsMap;

public:
	UFUNCTION(BlueprintCallable)
	void RemoveEntryByWidget(UDreamSequenceSubtitleEntryWidget* EntryWidget);

	UFUNCTION(BlueprintCallable)
	UDreamSequenceSubtitleEntryWidget* AddEntry(FDreamSequenceSubtitleSectionContent Entry);

	UFUNCTION(BlueprintImplementableEvent)
	void AddEntryWidgetTo(UDreamSequenceSubtitleEntryWidget* EntryWidget);

	virtual void Activated();
	virtual void Deactivated();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Activated")
	void BP_Activated();

	UFUNCTION(BlueprintNativeEvent, DisplayName = "Deactivated")
	void BP_Deactivated();

public:
	UPROPERTY(BlueprintAssignable)
	FOnEntryChanged OnEntryChanged;

	UPROPERTY(BlueprintReadOnly, Transient, Meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_In;

	UPROPERTY(BlueprintReadOnly, Transient, Meta = (BindWidgetAnimOptional))
	UWidgetAnimation* Animation_Out;
};
