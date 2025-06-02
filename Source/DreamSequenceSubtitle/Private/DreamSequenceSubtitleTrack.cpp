#include "DreamSequenceSubtitleTrack.h"

#include "DreamSequenceSubtitleTemplate.h"

#define LOCTEXT_NAMESPACE "UDreamSequenceSubtitleTrack"

UDreamSequenceSubtitleTrack::UDreamSequenceSubtitleTrack(const FObjectInitializer& ObjectInitializer)
{
}

UDreamSequenceSubtitleTrack::~UDreamSequenceSubtitleTrack()
{
}

bool UDreamSequenceSubtitleTrack::IsEmpty() const
{
	return Sections.IsEmpty();
}

void UDreamSequenceSubtitleTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}

void UDreamSequenceSubtitleTrack::RemoveSection(UMovieSceneSection& Section)
{
	Sections.Remove(&Section);
}

UMovieSceneSection* UDreamSequenceSubtitleTrack::CreateNewSection()
{
	UDreamSequenceSubtitleSection* Section = NewObject<UDreamSequenceSubtitleSection>(this, UDreamSequenceSubtitleSection::StaticClass(), NAME_None, RF_Transactional);

	return Section;
}

const TArray<UMovieSceneSection*>& UDreamSequenceSubtitleTrack::GetAllSections() const
{
	return Sections;
}

FMovieSceneEvalTemplatePtr UDreamSequenceSubtitleTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FDreamSequenceSubtitleSectionTemplate(*CastChecked<UDreamSequenceSubtitleSection>(&InSection));
}

bool UDreamSequenceSubtitleTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}

#if WITH_EDITOR
FText UDreamSequenceSubtitleTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("DisplayName", "Subtitle");
}
#endif

UDreamSequenceSubtitleSection* UDreamSequenceSubtitleTrack::GetSectionByID(int id)
{
	for (UMovieSceneSection* Element : Sections)
	{
		UDreamSequenceSubtitleSection* Section = Cast<UDreamSequenceSubtitleSection>(Element);

		if (Section && Section->GetUniqueID() == id)
		{
			return Section;
		}
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
