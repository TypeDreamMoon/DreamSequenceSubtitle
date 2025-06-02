// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamSequenceSubtitleLog.h"
#include "Subsystems/WorldSubsystem.h"
#include "DreamSequenceSubtitleWorldSubsystem.generated.h"

class UDreamSequenceSubtitleWidget;
/**
 * 
 */
UCLASS()
class DREAMSEQUENCESUBTITLE_API UDreamSequenceSubtitleWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UDreamSequenceSubtitleWorldSubsystem* Get(UWorld* InWorld);

	virtual void Deinitialize() override;
public:
	UPROPERTY()
	TObjectPtr<UDreamSequenceSubtitleWidget> SubtitleWidget;

	UDreamSequenceSubtitleWidget* GetSubtitleWidget();
	void SetSubtitleWidget(UDreamSequenceSubtitleWidget* InSubtitleWidget);
	bool SubtitleWidgetIsVaild();

	UPROPERTY()
	int32 RefCount = 0;

	void AddRef()
	{
		RefCount++;
		DSS_LOG(Log, TEXT("AddRef RefCount: %d"), RefCount)
		RefreshView(GWorld);
	}

	void Release()
	{
		RefCount--;
		DSS_LOG(Log, TEXT("Release RefCount: %d"), RefCount)
		RefreshView(GWorld);
	}

	bool IsRef()
	{
		return RefCount > 0;
	}

	bool IsNotRef()
	{
		return RefCount <= 0;
	}

	void RefreshView(UWorld* InWorld);

private:
	UDreamSequenceSubtitleWidget* CreateSubtitleWidget(UWorld* InWorld);

#if WITH_EDITOR
	TWeakPtr<SWidget> ViewportWidget;
#endif
};
