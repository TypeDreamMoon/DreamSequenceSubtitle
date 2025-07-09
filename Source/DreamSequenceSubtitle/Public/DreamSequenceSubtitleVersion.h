#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

struct FDreamSequenceSubtitleVersion
{
	enum Type
	{
		BeforeVersionWasAdded = 0,

		V1 = 1,

		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	const static FGuid GUID;

private:
	FDreamSequenceSubtitleVersion()
	{
	}
};