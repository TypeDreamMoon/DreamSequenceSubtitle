#include "DreamSequenceSubtitleEditorStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FDreamSequenceSubtitleStyle::StyleInstance = nullptr;

void FDreamSequenceSubtitleStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FDreamSequenceSubtitleStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FDreamSequenceSubtitleStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("DreamSequenceSubtitleStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.f, 20.f);
const FVector2D Icon40x40(40.f, 40.f);

TSharedRef<class FSlateStyleSet> FDreamSequenceSubtitleStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("DreamSequenceSubtitleStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("DreamSequenceSubtitle")->GetBaseDir() / TEXT("Resources" / TEXT("Icons")));

	// Add Style...

	Style->Set("DreamSequenceSubtitle.Track", new IMAGE_BRUSH(TEXT("Subtitle"), Icon20x20));
	
	return Style;
}

void FDreamSequenceSubtitleStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FDreamSequenceSubtitleStyle::Get()
{
	return *StyleInstance;
}
