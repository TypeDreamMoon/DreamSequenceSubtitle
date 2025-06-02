#include "DreamSequenceSubtitleEditor.h"

#include "DreamSequenceSubtitleEditorSection.h"
#include "SequencerUtilities.h"
#include "DreamSequenceSubtitleTrack.h"
#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "DreamSequenceSubtitleEditor"

FDreamSequenceSubtitleEditor::FDreamSequenceSubtitleEditor(TSharedRef<ISequencer> InSequencer) : FMovieSceneTrackEditor(InSequencer)
{
}

FDreamSequenceSubtitleEditor::~FDreamSequenceSubtitleEditor()
{
}

TSharedRef<ISequencerTrackEditor> FDreamSequenceSubtitleEditor::CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer)
{
	return MakeShareable(new FDreamSequenceSubtitleEditor(OwningSequencer));
}

TSharedRef<ISequencerSection> FDreamSequenceSubtitleEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	UDreamSequenceSubtitleSection* Section = Cast<UDreamSequenceSubtitleSection>(&SectionObject);
	check(SupportsType( SectionObject.GetOuter()->GetClass() ) && Section != nullptr);

	return MakeShareable(new FDreamSequenceSubtitleMovieSceneSection(*Section));
}

bool FDreamSequenceSubtitleEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UDreamSequenceSubtitleTrack::StaticClass();
}

const FSlateBrush* FDreamSequenceSubtitleEditor::GetIconBrush() const
{
	return FAppStyle::GetBrush("Sequencer.Tracks.LevelVisibility");
}

void FDreamSequenceSubtitleEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();

	if ((RootMovieSceneSequence == nullptr) || (RootMovieSceneSequence->GetClass()->GetName() != TEXT("LevelSequence")))
	{
		return;
	}

	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddSubtitleTrack", "Subtitle"),
		LOCTEXT("AddSubtitleToolTip", "Subtitle Track."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Sequencer.Tracks.LevelVisibility"),
		FUIAction(FExecuteAction::CreateRaw(this, &FDreamSequenceSubtitleEditor::OnAddTrack)));
}

TSharedPtr<SWidget> FDreamSequenceSubtitleEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	return FSequencerUtilities::MakeAddButton(
		LOCTEXT("AddDialogueTrigger", "Add Dialogue"),
		FOnGetContent::CreateSP(this, &FDreamSequenceSubtitleEditor::BuildAddVisibilityTriggerMenu, Track),
		Params.NodeIsHovered,
		GetSequencer());
}

void FDreamSequenceSubtitleEditor::BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track)
{
	UDreamSequenceSubtitleTrack* DialogueTrack = Cast<UDreamSequenceSubtitleTrack>(Track);

	class FSubtitleTrackCustomization : public IDetailCustomization
	{
	public:
		FSubtitleTrackCustomization()
		{
		}

		~FSubtitleTrackCustomization()
		{
		}

		virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
		{
			DetailBuilder.HideCategory("Track");
			DetailBuilder.HideCategory("General");
		}
	};

	auto SubMenuDelegate = [DialogueTrack](FMenuBuilder& InMenuBuilder, UDreamSequenceSubtitleTrack* InDialogueTrack)
	{
		FDetailsViewArgs DetailsViewArgs;
		{
			DetailsViewArgs.bAllowSearch = false;
			DetailsViewArgs.bCustomFilterAreaLocation = true;
			DetailsViewArgs.bCustomNameAreaLocation = true;
			DetailsViewArgs.bHideSelectionTip = true;
			DetailsViewArgs.bLockable = false;
			DetailsViewArgs.bSearchInitialKeyFocus = true;
			DetailsViewArgs.bUpdatesFromSelection = false;
			DetailsViewArgs.bShowOptions = false;
			DetailsViewArgs.bShowModifiedPropertiesOption = false;
		}

		TArray<TWeakObjectPtr<UObject>> Tracks;
		{
			Tracks.Add(DialogueTrack);
		}

		TSharedRef<IDetailsView> DetailsView = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor").CreateDetailView(DetailsViewArgs);

		FOnGetDetailCustomizationInstance CreateInstance = FOnGetDetailCustomizationInstance::CreateLambda([=] { return MakeShared<FSubtitleTrackCustomization>(); });
		DetailsView->RegisterInstancedCustomPropertyLayout(UDreamSequenceSubtitleTrack::StaticClass(), CreateInstance);
		{
			DetailsView->SetObjects(Tracks);
		}

		InMenuBuilder.AddWidget(DetailsView, FText::GetEmpty(), true);
	};

	const bool bInOpenSubMenuOnClick = false;

	MenuBuilder.AddSubMenu(
		LOCTEXT("Properties", "Properties"),
		LOCTEXT("PropertiesTooltip", ""),
		FNewMenuDelegate::CreateLambda(SubMenuDelegate, DialogueTrack),
		bInOpenSubMenuOnClick);
}

void FDreamSequenceSubtitleEditor::AddNewSection(UMovieScene* MovieScene, UMovieSceneTrack* LevelVisibilityTrack)
{
	const FScopedTransaction Transaction(LOCTEXT("AddDialogueSection_Transaction", "Add Dialogue Trigger"));
	LevelVisibilityTrack->Modify();

	FFrameNumber startSliderTime = MovieScene->GetPlaybackRange().GetLowerBoundValue();

	FFrameNumber LastSectionEndTime = startSliderTime;
	for (auto Section : LevelVisibilityTrack->GetAllSections())
	{
		TRange<FFrameNumber> SectionRange = Section->GetRange();
		LastSectionEndTime = SectionRange.GetUpperBoundValue() > LastSectionEndTime ? SectionRange.GetUpperBoundValue() : LastSectionEndTime;
	}

	UDreamSequenceSubtitleSection* SubtitleSection = CastChecked<UDreamSequenceSubtitleSection>(LevelVisibilityTrack->CreateNewSection());
	if (LevelVisibilityTrack->GetAllSections().Num() > 0)
	{
		UDreamSequenceSubtitleSection* LastSection = Cast<UDreamSequenceSubtitleSection>(LevelVisibilityTrack->GetAllSections().Last());
		SubtitleSection->SetContent(LastSection->GetContent());
	}

	startSliderTime = LastSectionEndTime;
	FFrameRate TickResolution = MovieScene->GetTickResolution();
	FFrameNumber SectionLength = (0.5 * TickResolution).FloorToFrame();
	FFrameNumber endTime = startSliderTime + SectionLength;
	if (endTime < startSliderTime)
	{
		FFrameNumber Temp = startSliderTime;
		startSliderTime = endTime;
		endTime = Temp;
	}

	SubtitleSection->SetRange(TRange<FFrameNumber>(startSliderTime, TRangeBound<FFrameNumber>::Exclusive(endTime)));

	SubtitleSection->SetRowIndex(0);

	LevelVisibilityTrack->AddSection(*SubtitleSection);
}

void FDreamSequenceSubtitleEditor::OnAddTrack()
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr)
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("AddDialogueTrack_Transaction", "Add Dialogue Track"));
	FocusedMovieScene->Modify();

	UDreamSequenceSubtitleTrack* NewTrack = FocusedMovieScene->AddTrack<UDreamSequenceSubtitleTrack>();
	checkf(NewTrack != nullptr, TEXT("Failed to create new dialogue track."));

	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

TSharedRef<SWidget> FDreamSequenceSubtitleEditor::BuildAddVisibilityTriggerMenu(UMovieSceneTrack* LevelVisibilityTrack)
{
	FMenuBuilder MenuBuilder(true, nullptr);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddSubtitleTrigger", "Subtitle"),
		LOCTEXT("AddSubtitleTriggerToolTip", "Add a Subtitle."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateSP(
			this, &FDreamSequenceSubtitleEditor::OnAddNewSection, LevelVisibilityTrack)));

	return MenuBuilder.MakeWidget();
}

void FDreamSequenceSubtitleEditor::OnAddNewSection(UMovieSceneTrack* LevelVisibilityTrack)
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr)
	{
		return;
	}

	AddNewSection(FocusedMovieScene, LevelVisibilityTrack);
	GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}

#undef LOCTEXT_NAMESPACE
