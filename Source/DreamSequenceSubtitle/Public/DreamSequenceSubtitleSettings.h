// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DreamSequenceSubtitleSettings.generated.h"

class UDreamSequenceSubtitleEntryWidget;
class UDreamSequenceSubtitleWidget;
/**
 * 
 */
UCLASS(Config=DreamSequenceSubtitleSettings, DefaultConfig)
class DREAMSEQUENCESUBTITLE_API UDreamSequenceSubtitleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDreamSequenceSubtitleSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual FName GetCategoryName() const override { return TEXT("DreamPlugin"); }
	virtual FName GetContainerName() const override { return TEXT("Project"); }
	virtual FName GetSectionName() const override { return TEXT("DreamSequenceSubtitle"); }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Classes")
	TSoftClassPtr<UDreamSequenceSubtitleWidget> SubtitleWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Classes")
	TSoftClassPtr<UDreamSequenceSubtitleEntryWidget> SubtitleEntryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Animation")
	bool bSubtitleWidgetAnimation = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Animation")
	bool bSubtitleWidgetEntryAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, Category = "Debug")
	bool bEnableDebug = false;

public:
	static void Register();
	static void Unregister();
	static UDreamSequenceSubtitleSettings* Get();

	TSubclassOf<UDreamSequenceSubtitleWidget> GetSubtitleWidgetClass();
	TSubclassOf<UDreamSequenceSubtitleEntryWidget> GetSubtitleEntryWidgetClass();
};
