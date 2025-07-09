#include "DreamSequenceSubtitleSection.h"

#include "DreamSequenceSubtitleVersion.h"
#include "Kismet/KismetSystemLibrary.h"

FDreamSequenceSubtitleSectionContent UDreamSequenceSubtitleSection::GetContent() const
{
	UKismetSystemLibrary::PrintString(GWorld, FString::Printf(TEXT("SS: %s %s"), *Speaker.ToString(), *Content.ToString()));
	return FDreamSequenceSubtitleSectionContent(Speaker, Content);
}

void UDreamSequenceSubtitleSection::SetContent(const FDreamSequenceSubtitleSectionContent& InContent)
{
	{
		Speaker = InContent.Speaker;
		Content = InContent.Content;
	}
}

void UDreamSequenceSubtitleSection::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FDreamSequenceSubtitleVersion::GUID);

	if (Ar.CustomVer(FDreamSequenceSubtitleVersion::GUID) >= FDreamSequenceSubtitleVersion::V1)
	{
		if (Ar.IsLoading())
		{
			Ar << Speaker;
			Ar << Content;
		}

		if (Ar.IsSaving())
		{
			Ar << Speaker;
			Ar << Content;
		}
	}
}
