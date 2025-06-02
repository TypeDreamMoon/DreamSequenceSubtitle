#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

class FMenuBuilder;

class DREAMSEQUENCESUBTITLEEDITOR_API FDreamSequenceSubtitleEditor : public FMovieSceneTrackEditor
{
public:
	FDreamSequenceSubtitleEditor(TSharedRef<ISequencer> InSequencer);

	virtual ~FDreamSequenceSubtitleEditor() override;
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	virtual const FSlateBrush* GetIconBrush() const override;
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;

	virtual void BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track) override;

private:
	void AddNewSection(UMovieScene* MovieScene, UMovieSceneTrack* LevelVisibilityTrack);
	/** Handles when the add track menu item is activated. */
	void OnAddTrack();

	TSharedRef<SWidget> BuildAddVisibilityTriggerMenu(UMovieSceneTrack* LevelVisibilityTrack);

	void OnAddNewSection(UMovieSceneTrack* LevelVisibilityTrack);
};
