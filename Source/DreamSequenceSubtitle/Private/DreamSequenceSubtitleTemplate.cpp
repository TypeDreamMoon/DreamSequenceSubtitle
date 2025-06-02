#include "DreamSequenceSubtitleTemplate.h"

#include "DreamSequenceSubtitleLog.h"
#include "DreamSequenceSubtitleWorldSubsystem.h"
#include "Components/DreamSequenceSubtitleWidget.h"
#include "Components/DreamSequenceSubtitleEntryWidget.h"


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

static TMap<FMovieSceneEvaluationOperand, FMovieSceneSharedDataId> OperandMapping;

static FSharedPersistentDataKey RegisterDataKey(const FMovieSceneEvaluationOperand& InOperand)
{
	if (!OperandMapping.Contains(InOperand))
	{
		OperandMapping.Add(InOperand, FMovieSceneSharedDataId::Allocate());
	}

	return FSharedPersistentDataKey(OperandMapping[InOperand], InOperand);
}

static FSharedPersistentDataKey GetSharedDataKey(const FMovieSceneEvaluationOperand& InOperand)
{
	RegisterDataKey(InOperand);

	return FSharedPersistentDataKey(OperandMapping[InOperand], InOperand);
}


FDreamSequenceSubtitleSectionTemplate::FDreamSequenceSubtitleSectionTemplate()
{
}

FDreamSequenceSubtitleSectionTemplate::FDreamSequenceSubtitleSectionTemplate(const UDreamSequenceSubtitleSection& Section) : Content(Section.GetContent())
{
}

void FDreamSequenceSubtitleSectionTemplate::Initialize(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	DSS_FLOG(Log, TEXT("Initialize Template"))
	RegisterDataKey(Operand);
}

void FDreamSequenceSubtitleSectionTemplate::SetupOverrides()
{
	EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag);
}

void FDreamSequenceSubtitleSectionTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	FMovieSceneSharedDataId ID = GetSharedDataKey(Operand).UniqueId;
	DSS_FLOG(Log, TEXT("Speaker: %15s Content: %15s"), *Content.Speaker.ToString(), *Content.Content.ToString())
	const FDreamSequenceSubtitleSharedTrackData* TrackData = PersistentData.Find<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(Operand));
	if (TrackData && TrackData->HasAnythingToDo() && !ExecutionTokens.FindShared(ID))
	{
		FDreamSequenceSubtitleToken Token = FDreamSequenceSubtitleToken(Operand, const_cast<FDreamSequenceSubtitleSectionTemplate&>(*this), Content);

#if WITH_EDITOR
		if (GIsEditor && !GIsPlayInEditorWorld)
		{
			Token.EnvMode = EDreamSequenceSubtitleEnvMode::WithEditor;
		}
#else
		Token.EnvMode = EDreamSequenceSubtitleEnvMode::WithRuntime;
#endif

		ExecutionTokens.AddShared(ID, Token);
	}
}

void FDreamSequenceSubtitleSectionTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	FDreamSequenceSubtitleSharedTrackData& TrackData = PersistentData.GetOrAdd<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(SelfOperand));
	TrackData.SetInfo();

	if (!GetEntry())
	{
		// TODO Entry Added
		DSS_FLOG(Log, TEXT("Add : Speaker: %s Content: %s"), *Content.Speaker.ToString(), *Content.Content.ToString())
		SetEntry(TrackData.AddEntry(Content));
	}
}

void FDreamSequenceSubtitleSectionTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	DSS_FLOG(Log, TEXT("Remove : Speaker: %s Content: %s"), *Content.Speaker.ToString(), *Content.Content.ToString())
	FDreamSequenceSubtitleSharedTrackData& TrackData = PersistentData.GetOrAdd<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(SelfOperand));
	TrackData.SetInfo();
	TrackData.RemoveEntry(EntryWidget);
	TrackData.ResetEntry(const_cast<FDreamSequenceSubtitleSectionTemplate&>(*this));
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
	if (EntryWidget && Subsystem->SubtitleWidgetIsVaild())
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

	Subsystem->AddRef();
	UDreamSequenceSubtitleEntryWidget* Widget = Subsystem->GetSubtitleWidget()->AddEntry(InNewContent);

	return Widget;
}

/* -------------------------------------------------------------------------
 *
 *		FDreamSequenceSubtitleToken
 *
 * ------------------------------------------------------------------------- */

FDreamSequenceSubtitleToken::FDreamSequenceSubtitleToken(const FMovieSceneEvaluationOperand& InOperand, FDreamSequenceSubtitleSectionTemplate& InTemplate, const FDreamSequenceSubtitleSectionContent& InContent)
	: Operand(InOperand), Template(InTemplate), Content(InContent)
{
}

void FDreamSequenceSubtitleToken::Execute(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player)
{
	FDreamSequenceSubtitleSharedTrackData* TrackData = PersistentData.Find<FDreamSequenceSubtitleSharedTrackData>(GetSharedDataKey(Operand));
	if (TrackData)
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
