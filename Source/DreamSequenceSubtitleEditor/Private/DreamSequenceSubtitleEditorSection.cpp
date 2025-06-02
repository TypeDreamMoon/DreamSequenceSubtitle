#include "DreamSequenceSubtitleEditorSection.h"

#include "Editor/EditorWidgets/Public/SDropTarget.h"
#include "SequencerSectionPainter.h"

FDreamSequenceSubtitleMovieSceneSection::FDreamSequenceSubtitleMovieSceneSection(UDreamSequenceSubtitleSection& InSectionObject) : SectionObject(InSectionObject)
{
}

UMovieSceneSection* FDreamSequenceSubtitleMovieSceneSection::GetSectionObject()
{
	return &SectionObject;
}

TSharedRef<SWidget> FDreamSequenceSubtitleMovieSceneSection::GenerateSectionWidget()
{
	return
			SNew(SDropTarget)
			.OnAllowDrop(this, &FDreamSequenceSubtitleMovieSceneSection::OnAllowDrop)
			.OnDropped(this, &FDreamSequenceSubtitleMovieSceneSection::OnDrop)
			.Content()
			[
				SNew(SBorder)
				.BorderBackgroundColor(this, &FDreamSequenceSubtitleMovieSceneSection::GetBackgroundColor)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				[
					SNew(SVerticalBox)
					.ToolTipText(this, &FDreamSequenceSubtitleMovieSceneSection::GetVisibilityTooltip)

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &FDreamSequenceSubtitleMovieSceneSection::GetVisibilitySpeakerText)
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(this, &FDreamSequenceSubtitleMovieSceneSection::GetVisibilityText)
					]
				]
			];
}

int32 FDreamSequenceSubtitleMovieSceneSection::OnPaintSection(FSequencerSectionPainter& InPainter) const
{
	return InPainter.PaintSectionBackground();
}

FText FDreamSequenceSubtitleMovieSceneSection::GetSectionTitle() const
{
	return FText();
}

float FDreamSequenceSubtitleMovieSceneSection::GetSectionHeight(const UE::Sequencer::FViewDensityInfo& ViewDensity) const
{
	return 50.f;
}

FSlateColor FDreamSequenceSubtitleMovieSceneSection::GetBackgroundColor() const
{
	return FSlateColor(FLinearColor::Blue.Desaturate(0.5f));
}

FText FDreamSequenceSubtitleMovieSceneSection::GetVisibilitySpeakerText() const
{
	return FText::FromString(
		TEXT("Speaker: ") +
		SectionObject.GetContent().Speaker.ToString()
	);
}

FText FDreamSequenceSubtitleMovieSceneSection::GetVisibilityText() const
{
	return FText::FromString(
		TEXT("Content: ") +
		SectionObject.GetContent().Content.ToString()
	);
}

FText FDreamSequenceSubtitleMovieSceneSection::GetVisibilityTooltip() const
{
	return FText::Format(NSLOCTEXT("DreamSequenceSubtitleTrackEditor", "SubtitleSectionText", " Speaker: {0} :\n Content: {1}"), SectionObject.GetContent().Speaker, SectionObject.GetContent().Content);
}

bool FDreamSequenceSubtitleMovieSceneSection::OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation)
{
	return false;
}

FReply FDreamSequenceSubtitleMovieSceneSection::OnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent)
{
	return FReply::Unhandled();
}
