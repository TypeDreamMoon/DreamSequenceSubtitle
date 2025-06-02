#pragma once

#include "CoreMinimal.h"
#include "DreamSequenceSubtitleTypes.generated.h"

#define LOCTEXT_NAMESPACE "DreamSequenceSubtitleTypes"

USTRUCT(BlueprintType)
struct FDreamSequenceSubtitleSectionContent
{
	GENERATED_BODY()

public:
	FDreamSequenceSubtitleSectionContent()
	{
		Speaker = LOCTEXT("SpeakerDefaultName", "Speaker");
		Content = LOCTEXT("DefaultContent", "Hello World");
	}

	FDreamSequenceSubtitleSectionContent(const FText& InSpeaker, const FText& InContent)
	{
		Speaker = InSpeaker;
		Content = InContent;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Speaker = LOCTEXT("SpeakerDefaultName", "Speaker");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MultiLine))
	FText Content = LOCTEXT("DefaultContent", "Hello World");

	bool operator==(const FDreamSequenceSubtitleSectionContent& Other) const;
};

#undef LOCTEXT_NAMESPACE
