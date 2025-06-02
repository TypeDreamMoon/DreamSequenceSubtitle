// Copyright 2022 - 2025 Dream Moon Team. All Rights Reserved.


#include "DreamSequenceSubtitleWorldSubsystem.h"

#include "Components/DreamSequenceSubtitleWidget.h"
#include "DreamSequenceSubtitleSettings.h"
#include "Widgets/SViewport.h"
#include "Slate/SceneViewport.h"

UDreamSequenceSubtitleWorldSubsystem* UDreamSequenceSubtitleWorldSubsystem::Get(UWorld* InWorld)
{
	return InWorld->GetSubsystem<UDreamSequenceSubtitleWorldSubsystem>();
}

void UDreamSequenceSubtitleWorldSubsystem::Deinitialize()
{
#if WITH_EDITOR
	if (GIsEditor && !GWorld->IsPlayInEditor())
	{
		SOverlay* Overlay = static_cast<SOverlay*>(ViewportWidget.Pin().Get());
		if (Overlay)
		{
			Overlay->RemoveSlot(GetSubtitleWidget()->TakeWidget());
		}
	}
#else
	if (GEngine && GEngine->GameViewport)
	{
		GetSubtitleWidget()->RemoveFromParent();
	}
#endif
	SetSubtitleWidget(nullptr);
}

UDreamSequenceSubtitleWidget* UDreamSequenceSubtitleWorldSubsystem::GetSubtitleWidget()
{
	return SubtitleWidget;
}

void UDreamSequenceSubtitleWorldSubsystem::SetSubtitleWidget(UDreamSequenceSubtitleWidget* InSubtitleWidget)
{
	SubtitleWidget = InSubtitleWidget;
}

bool UDreamSequenceSubtitleWorldSubsystem::SubtitleWidgetIsVaild()
{
	return SubtitleWidget != nullptr;
}

void UDreamSequenceSubtitleWorldSubsystem::RefreshView(UWorld* InWorld)
{
	if (IsRef())
	{
		if (!SubtitleWidgetIsVaild())
		{
#if WITH_EDITOR // Editor Test Add Subtitle Widget To Editor Viewport
			if (GIsEditor && !InWorld->IsPlayInEditor())
			{
				const TIndirectArray<FWorldContext>& Contexts = GEditor->GetWorldContexts();

				if (!Contexts.IsEmpty())
				{
					FSceneViewport* SceneViewport = nullptr;
					UEditorEngine* EditorEngine = CastChecked<UEditorEngine>(GEngine);
					FSceneViewport* EditorViewport = static_cast<FSceneViewport*>(EditorEngine->GetActiveViewport());

					if (EditorViewport != nullptr)
					{
						SceneViewport = EditorViewport;
					}

					if (SceneViewport)
					{
						if (SceneViewport->GetViewportWidget().IsValid())
						{
							// Check Subtitle Widget Is Valid. If not, Create New One.
							if (!SubtitleWidgetIsVaild())
							{
								SetSubtitleWidget(CreateSubtitleWidget(InWorld));
							}

							TSharedRef<SWidget> Widget = SubtitleWidget->TakeWidget();
							ViewportWidget = SceneViewport->GetViewportWidget().Pin()->GetContent();
							SOverlay* Overlay = static_cast<SOverlay*>(ViewportWidget.Pin().Get());
							if (Overlay)
							{
								Overlay->AddSlot()[
									Widget
								];
							}
						}
					}
				}
			}
#else // Runtime Add To Viewport
		if (GEngine && GEngine->GameViewport)
		{
			if (!SubtitleWidgetIsVaild())
			{
				UDreamSequenceSubtitleWidget* Widget = CreateSubtitleWidget(InWorld);
				SetSubtitleWidget(Widget);
				GEngine->GameViewport->AddViewportWidgetContent(Widget->TakeWidget());
			}
		}
#endif
		}
	}
	else // Is Not Ref. Remove From Parent
	{
		if (SubtitleWidgetIsVaild())
		{
#if WITH_EDITOR
			if (GIsEditor && !InWorld->IsPlayInEditor())
			{
				SOverlay* Overlay = static_cast<SOverlay*>(ViewportWidget.Pin().Get());
				if (Overlay)
				{
					Overlay->RemoveSlot(GetSubtitleWidget()->TakeWidget());
				}
			}
#else
			if (GEngine && GEngine->GameViewport)
			{
				GetSubtitleWidget()->RemoveFromParent();
			}
#endif
			SetSubtitleWidget(nullptr);
		}
	}
}

UDreamSequenceSubtitleWidget* UDreamSequenceSubtitleWorldSubsystem::CreateSubtitleWidget(UWorld* InWorld)
{
	TSubclassOf<UDreamSequenceSubtitleWidget> Class = UDreamSequenceSubtitleSettings::Get()->GetSubtitleWidgetClass();
	if (Class)
	{
		return CreateWidget<UDreamSequenceSubtitleWidget>(InWorld, Class);
	}

	return nullptr;
}
