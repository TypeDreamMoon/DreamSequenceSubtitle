#include "DreamSequenceSubtitleVersion.h"
#include "Runtime/Core/Public/Serialization/CustomVersion.h"

// {443330B6-4FD8-411F-8ED2-7ABB529C8D60}
const FGuid FDreamSequenceSubtitleVersion::GUID = FGuid(0x443330B6, 0x4FD8411F, 0x8ED27ABB, 0x529C8D60);
FCustomVersionRegistration GRegisterDreamSequenceSubtitleVersion = FCustomVersionRegistration(
	FDreamSequenceSubtitleVersion::GUID, FDreamSequenceSubtitleVersion::LatestVersion, TEXT("V1"));