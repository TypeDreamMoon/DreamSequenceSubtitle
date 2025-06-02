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
					SNew(STextBlock)
					.Text(this, &FDreamSequenceSubtitleMovieSceneSection::GetVisibilityText)
					.ToolTipText(this, &FDreamSequenceSubtitleMovieSceneSection::GetVisibilityTooltip)
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

FSlateColor FDreamSequenceSubtitleMovieSceneSection::GetBackgroundColor() const
{
	return FSlateColor(FLinearColor::Green.Desaturate(.5f));
}

FText FDreamSequenceSubtitleMovieSceneSection::GetVisibilityText() const
{
	return FText::FromString(
		SectionObject.GetContent().Content.ToString() +
		FString::Printf(TEXT("ID : %s"), *SectionObject.GetName())
	);
}

FText FDreamSequenceSubtitleMovieSceneSection::GetVisibilityTooltip() const
{
	return FText::Format(NSLOCTEXT("DreamSequenceSubtitleTrackEditor", "SubtitleSectionText", "{0} :\n{1}"), SectionObject.GetContent().Speaker, SectionObject.GetContent().Content);
}

bool FDreamSequenceSubtitleMovieSceneSection::OnAllowDrop(TSharedPtr<FDragDropOperation> DragDropOperation)
{
	return false;
}

FReply FDreamSequenceSubtitleMovieSceneSection::OnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent)
{
	return FReply::Unhandled();
}
