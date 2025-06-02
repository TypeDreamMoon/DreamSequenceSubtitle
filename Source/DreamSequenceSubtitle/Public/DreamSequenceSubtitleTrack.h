// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.

#pragma once

#include "MovieSceneNameableTrack.h"
#include "Tracks/MovieSceneSpawnTrack.h"
#include "Tracks/MovieScenePropertyTrack.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "DreamSequenceSubtitleTrack.generated.h"

class UDreamSequenceSubtitleSection;

UCLASS()
class DREAMSEQUENCESUBTITLE_API UDreamSequenceSubtitleTrack : public UMovieSceneNameableTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

public:
	UDreamSequenceSubtitleTrack(const FObjectInitializer& ObjectInitializer);
	virtual ~UDreamSequenceSubtitleTrack() override;

public:
	static uint16 GetEvaluationPriority() { return UMovieSceneSpawnTrack::GetEvaluationPriority() + 120; }

public:
	virtual bool IsEmpty() const override;
	virtual void AddSection(UMovieSceneSection& Section) override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
	virtual bool HasSection(const UMovieSceneSection& Section) const override;
	virtual bool SupportsMultipleRows() const override { return true; }

#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
#endif

public:
	UDreamSequenceSubtitleSection* GetSectionByID(int id);

private:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};
