#pragma once

#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDreamSequenceSubtitle, All, All);

#define DSS_LOG(V, F, ...)  UE_LOG(LogDreamSequenceSubtitle, V, F, ##__VA_ARGS__)
#define DSS_FLOG(V, F, ...) UE_LOG(LogDreamSequenceSubtitle, V, TEXT("[%hs] " F), __FUNCTION__, ##__VA_ARGS__)