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

public:
	UPROPERTY(BlueprintAssignable)
	FOnEntryChanged OnEntryChanged;
};
