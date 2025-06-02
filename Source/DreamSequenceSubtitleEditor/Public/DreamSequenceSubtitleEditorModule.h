#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDreamSequenceSubtitleEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	FDelegateHandle Handle;
};
