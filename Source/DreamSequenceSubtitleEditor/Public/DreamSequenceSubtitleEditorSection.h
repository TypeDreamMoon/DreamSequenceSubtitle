#pragma once

#include "CoreMinimal.h"
#include "ISequencerSection.h"
#include "DreamSequenceSubtitleSection.h"

class DREAMSEQUENCESUBTITLEEDITOR_API FDreamSequenceSubtitleMovieSceneSection : public ISequencerSection, public TSharedFromThis<FDreamSequenceSubtitleMovieSceneSection>
{
public:
	FDreamSequenceSubtitleMovieSceneSection(UDreamSequenceSubtitleSection& InSectionObject);

public:
	virtual UMovieSceneSection* GetSectionObject() override;
	virtual TSharedRef<SWidget> GenerateSectionWidget() override;
	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override;
	virtual FText GetSectionTitle() const override;

private:
	FSlateColor GetBackgroundColor() const;
	FText GetVisibilityText() const;
	FText GetVisibilityTooltip() const;

	bool OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation);
	FReply OnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent);

private:
	UDreamSequenceSubtitleSection& SectionObject;
	FText DisplayName;
};
