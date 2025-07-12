#include "DreamSequenceSubtitleTemplate.h"

#include "DreamSequenceSubtitleLog.h"
#include "DreamSequenceSubtitleWorldSubsystem.h"
#include "Components/DreamSequenceSubtitleWidget.h"
#include "Components/DreamSequenceSubtitleEntryWidget.h"
#include "DreamSequenceSubtitleSettings.h"
#include "Kismet/KismetSystemLibrary.h"

#define DSS_DEBUG_LOG(V, F, ...) {if (UDreamSequenceSubtitleSettings* Settings = UDreamSequenceSubtitleSettings::Get()) {if (Settings->bEnableDebug){DSS_LOG(V, F, ##__VA_ARGS__)}}}

/* -------------------------------------------------------------------------
 *
 *		Global Static
 *
 * ------------------------------------------------------------------------- */

static TMap<FMovieSceneEvaluationOperand, FMovieSceneSharedDataId> OperandMapping;

static FSharedPersistentDataKey RegisterDataKey(const FMovieSceneEvaluationOperand& InOperand)
{
	if (!OperandMapping.Contains(InOperand))
	{
		OperandMapping.Add(InOperand, FMovieSceneSharedDataId::Allocate());
		DSS_DEBUG_LOG(Log, TEXT("Register Operand %d"), InOperand.SequenceID.GetInternalValue())
	}

	return FSharedPersistentDataKey(OperandMapping[InOperand], InOperand);
}

static FSharedPersistentDataKey GetSharedDataKey(const FMovieSceneEvaluationOperand& InOperand)
{
	RegisterDataKey(InOperand);

	return FSharedPersistentDataKey(OperandMapping[InOperand], InOperand);
}

/* -------------------------------------------------------------------------
 *
 *		FDreamSequenceSubtitleSharedTrackData
 *
 * ------------------------------------------------------------------------- */

USTRUCT()
struct FDreamSequenceSubtitleSharedTrackData : IPersistentEvaluationData
{
public:
	FDreamSequenceSubtitleSharedTrackData();
	~FDreamSequenceSubtitleSharedTrackData() override;

	bool HasAnythingToDo() const;
	void SetInfo();
	void Refresh(IMovieScenePlayer& Player);
	void RemoveEntry(UDreamSequenceSubtitleEntryWidget* EntryWidget);
	void ResetEntry(FDreamSequenceSubtitleSectionTemplate& SectionTemplate);
	UDreamSequenceSubtitleEntryWidget* AddEntry(FDreamSequenceSubtitleSectionContent InNewContent) const;

private:
	bool bNeedExecute = false;


	FDreamSequenceSubtitleSectionContent Content;

#if WITH_EDITOR
	TWeakPtr<SWidget> ViewportWidget;
#endif
};

/* -------------------------------------------------------------------------
 *
 *		FDreamSequenceSubtitleSectionTemplate
 *
 * ------------------------------------------------------------------------- */


FDreamSequenceSubtitleSectionTemplate::FDreamSequenceSubtitleSectionTemplate()
{
}

FDreamSequenceSubtitleSectionTemplate::FDreamSequenceSubtitleSectionTemplate(const UDreamSequenceSubtitleSection& Section) : Content(Section.GetContent())
{
	if (UDreamSequenceSubtitleSettings::Get()->bEnableDebug)
		UKismetSystemLibrary::PrintString(GWorld, FString::Printf(TEXT("Initialize Template %s SecC: %s %s"), *Section.GetName(), *Content.Speaker.ToString(), *Content.Content.ToString()), true);
}

void FDreamSequenceSubtitleSectionTemplate::Initialize(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	DSS_DEBUG_LOG(Log, TEXT("Initialize Template"));
	
	RegisterDataKey(Operand);
}

void FDreamSequenceSubtitleSectionTemplate::SetupOverrides()
{
	EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag | RequiresInitializeFlag);
}

void FDreamSequenceSubtitleSectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	const FDreamSequenceSubtitleSharedTrackData* TrackData = PersistentData.Find<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(Operand));
	if (TrackData && TrackData->HasAnythingToDo())
	{
		FDreamSequenceSubtitleToken Token = FDreamSequenceSubtitleToken(const_cast<FDreamSequenceSubtitleSectionTemplate&>(*this));

#if WITH_EDITOR
		if (GIsEditor && !GIsPlayInEditorWorld)
		{
			Token.EnvMode = EDreamSequenceSubtitleEnvMode::WithEditor;
		}
		else
#endif
		{
			Token.EnvMode = EDreamSequenceSubtitleEnvMode::WithRuntime;
		}

		ExecutionTokens.Add(Token);
	}
}

void FDreamSequenceSubtitleSectionTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FDreamSequenceSubtitleSharedTrackData& TrackData = PersistentData.GetOrAdd<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(SelfOperand));
	TrackData.SetInfo();
	// PersistentData.GetOrAddSectionData<>()

	// TODO : Entry Added
	if (!GetEntry())
	{
		DSS_DEBUG_LOG(Log, TEXT("Add : Speaker: %s Content: %s"), *Content.Speaker.ToString(), *Content.Content.ToString())
		SetEntry(TrackData.AddEntry(Content));
	}
}

void FDreamSequenceSubtitleSectionTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FDreamSequenceSubtitleSharedTrackData& TrackData = PersistentData.GetOrAdd<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(SelfOperand));
	TrackData.SetInfo();

	// TODO : Entry Remove
	if (GetEntry())
	{
		DSS_DEBUG_LOG(Log, TEXT("Remove : Speaker: %s Content: %s"), *Content.Speaker.ToString(), *Content.Content.ToString())
		TrackData.RemoveEntry(EntryWidget);
		TrackData.ResetEntry(const_cast<FDreamSequenceSubtitleSectionTemplate&>(*this));
	}
}

UScriptStruct& FDreamSequenceSubtitleSectionTemplate::GetScriptStructImpl() const
{
	return *StaticStruct();
}

void FDreamSequenceSubtitleSectionTemplate::SetEntry(UDreamSequenceSubtitleEntryWidget* InEntryWidget)
{
	EntryWidget = InEntryWidget;
}

void FDreamSequenceSubtitleSectionTemplate::SetEntry(UDreamSequenceSubtitleEntryWidget* InEntryWidget) const
{
	const_cast<FDreamSequenceSubtitleSectionTemplate*>(this)->EntryWidget = InEntryWidget;
}

/* -------------------------------------------------------------------------
 *
 *		FDreamSequenceSubtitleSharedTrackData
 *
 * ------------------------------------------------------------------------- */

FDreamSequenceSubtitleSharedTrackData::FDreamSequenceSubtitleSharedTrackData() : bNeedExecute(false)
{
}

FDreamSequenceSubtitleSharedTrackData::~FDreamSequenceSubtitleSharedTrackData()
{
}

bool FDreamSequenceSubtitleSharedTrackData::HasAnythingToDo() const
{
	return bNeedExecute;
}

void FDreamSequenceSubtitleSharedTrackData::SetInfo()
{
	bNeedExecute = true;
}

void FDreamSequenceSubtitleSharedTrackData::Refresh(IMovieScenePlayer& Player)
{
	bNeedExecute = false;

	UWorld* World = nullptr;

	if (Player.GetPlaybackContext())
	{
		World = Player.GetPlaybackContext()->GetWorld();
	}
	else
	{
		return;
	}

	UDreamSequenceSubtitleWorldSubsystem* Subsystem = UDreamSequenceSubtitleWorldSubsystem::Get(World);
	Subsystem->RefreshView(World);
}

void FDreamSequenceSubtitleSharedTrackData::RemoveEntry(UDreamSequenceSubtitleEntryWidget* EntryWidget)
{
	UDreamSequenceSubtitleWorldSubsystem* Subsystem = UDreamSequenceSubtitleWorldSubsystem::Get(GWorld);
	if (EntryWidget && Subsystem->SubtitleWidgetIsValid())
	{
		Subsystem->GetSubtitleWidget()->RemoveEntryByWidget(EntryWidget);
		Subsystem->Release();
	}
}

void FDreamSequenceSubtitleSharedTrackData::ResetEntry(FDreamSequenceSubtitleSectionTemplate& SectionTemplate)
{
	SectionTemplate.SetEntry(nullptr);
}

UDreamSequenceSubtitleEntryWidget* FDreamSequenceSubtitleSharedTrackData::AddEntry(FDreamSequenceSubtitleSectionContent InNewContent) const
{
	UDreamSequenceSubtitleWorldSubsystem* Subsystem = UDreamSequenceSubtitleWorldSubsystem::Get(GWorld);

	if (UDreamSequenceSubtitleSettings::Get()->bEnableDebug)
		UKismetSystemLibrary::PrintString(GWorld, FString::Printf(TEXT("Add : Speaker: %s Content: %s"), *InNewContent.Speaker.ToString(), *InNewContent.Content.ToString()));

	Subsystem->AddRef();
	UDreamSequenceSubtitleEntryWidget* Widget = Subsystem->GetSubtitleWidget()->AddEntry(InNewContent);

	return Widget;
}

/* -------------------------------------------------------------------------
 *
 *		FDreamSequenceSubtitleToken
 *
 * ------------------------------------------------------------------------- */

FDreamSequenceSubtitleToken::FDreamSequenceSubtitleToken(FDreamSequenceSubtitleSectionTemplate& InTemplate)
	: Template(InTemplate)
{
}

void FDreamSequenceSubtitleToken::Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player)
{
	// TODO : Refresh Subtitle Widget.
	if (FDreamSequenceSubtitleSharedTrackData* TrackData = PersistentData.Find<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(Operand)))
	{
#if WITH_EDITOR
		if (GIsEditor && !Player.GetPlaybackContext()->GetWorld()->IsPlayInEditor())
		{
			if (EnvMode == EDreamSequenceSubtitleEnvMode::WithEditor)
			{
				TrackData->Refresh(Player);
			}
		}
		else
#endif
		{
			if (EnvMode == EDreamSequenceSubtitleEnvMode::WithRuntime)
			{
				TrackData->Refresh(Player);
			}
		}
	}
}
