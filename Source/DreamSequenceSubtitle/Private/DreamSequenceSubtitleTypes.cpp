#include "DreamSequenceSubtitleTypes.h"

bool FDreamSequenceSubtitleSectionContent::operator==(const FDreamSequenceSubtitleSectionContent& Other) const
{
	return Content.ToString() == Other.Content.ToString() && Speaker.ToString() == Other.Speaker.ToString();
}
