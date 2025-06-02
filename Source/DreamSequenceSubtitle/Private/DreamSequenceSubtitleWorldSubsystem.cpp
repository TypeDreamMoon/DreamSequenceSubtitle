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

void UDreamSequenceSubtitleWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	RefreshView(GetWorld());
}

void UDreamSequenceSubtitleWorldSubsystem::Deinitialize()
{
#if WITH_EDITOR
	if (GIsEditor && !GWorld->IsPlayInEditor())
	{
		SOverlay* Overlay = static_cast<SOverlay*>(ViewportWidget.Pin().Get());
		if (Overlay && GetSubtitleWidget())
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

bool UDreamSequenceSubtitleWorldSubsystem::SubtitleWidgetIsValid()
{
	return SubtitleWidget != nullptr;
}

void UDreamSequenceSubtitleWorldSubsystem::RefreshView(UWorld* InWorld)
{
	if (IsRef())
	{
		if (!SubtitleWidgetIsValid())
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
							if (!SubtitleWidgetIsValid())
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
			else
#endif
			{
				if (GEngine && GEngine->GameViewport)
				{
					UDreamSequenceSubtitleWidget* Widget = CreateSubtitleWidget(InWorld);
					SetSubtitleWidget(Widget);
					GEngine->GameViewport->AddViewportWidgetContent(Widget->TakeWidget());
				}
			}
		}
	}
	else // Is Not Ref. Remove From Parent
	{
		if (SubtitleWidgetIsValid())
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
			else
#endif
			{
				if (GEngine && GEngine->GameViewport)
				{
					GetSubtitleWidget()->RemoveFromParent();
				}
			}

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
