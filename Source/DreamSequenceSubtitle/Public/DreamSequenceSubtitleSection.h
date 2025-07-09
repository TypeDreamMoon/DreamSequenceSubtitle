// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DreamSequenceSubtitleTypes.h"
#include "MovieSceneSection.h"
#include "DreamSequenceSubtitleSection.generated.h"

UCLASS()
class DREAMSEQUENCESUBTITLE_API UDreamSequenceSubtitleSection : public UMovieSceneSection
{
	GENERATED_BODY()

public:
	FDreamSequenceSubtitleSectionContent GetContent() const;

	void SetContent(const FDreamSequenceSubtitleSectionContent& InContent);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subtitle")
	FText Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subtitle")
	FText Content;
	
	virtual void Serialize(FArchive& Ar) override;
};
