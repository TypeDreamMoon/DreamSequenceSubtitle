// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamSequenceSubtitleTypes.h"
#include "MovieSceneSection.h"
#include "DreamSequenceSubtitleSection.generated.h"

UCLASS(MinimalAPI)
class UDreamSequenceSubtitleSection : public UMovieSceneSection
{
	GENERATED_BODY()

public:
	FDreamSequenceSubtitleSectionContent GetContent() const
	{
		return Content;
	}

	void SetContent(const FDreamSequenceSubtitleSectionContent& InContent)
	{
		Content = InContent;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subtitle")
	FDreamSequenceSubtitleSectionContent Content;
};
