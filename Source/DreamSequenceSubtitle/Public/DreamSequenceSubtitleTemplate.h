#pragma once

#include "CoreMinimal.h"
#include "Evaluation/PersistentEvaluationData.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "DreamSequenceSubtitleSection.h"
#include "DreamSequenceSubtitleTypes.h"
#include "Components/DreamSequenceSubtitleWidget.h"
#include "DreamSequenceSubtitleTemplate.generated.h"

class UDreamSequenceSubtitleEntryWidget;

USTRUCT()
struct FDreamSequenceSubtitleSectionTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()

public:
	FDreamSequenceSubtitleSectionTemplate();
	FDreamSequenceSubtitleSectionTemplate(const UDreamSequenceSubtitleSection& Section);

	void SetEntry(UDreamSequenceSubtitleEntryWidget* InEntryWidget);
	void SetEntry(UDreamSequenceSubtitleEntryWidget* InEntryWidget) const;
	UDreamSequenceSubtitleEntryWidget* GetEntry() const { return EntryWidget; }
private:
	virtual void Initialize(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void SetupOverrides() override;
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	
	virtual UScriptStruct& GetScriptStructImpl() const override;
private:
	FMovieSceneEvaluationOperand SelfOperand;
	FDreamSequenceSubtitleSectionContent Content;
	UPROPERTY()
	UDreamSequenceSubtitleEntryWidget* EntryWidget;
};

UENUM()
enum class EDreamSequenceSubtitleEnvMode
{
	None = 0,
	WithEditor = 1,
	WithRuntime = 2,
};

struct FDreamSequenceSubtitleToken : public IMovieSceneSharedExecutionToken
{
public:
	FDreamSequenceSubtitleToken(const FMovieSceneEvaluationOperand& InOperand, FDreamSequenceSubtitleSectionTemplate& InTemplate, const FDreamSequenceSubtitleSectionContent& Content);

public:
	virtual void Execute(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override;

public:
	EDreamSequenceSubtitleEnvMode EnvMode = EDreamSequenceSubtitleEnvMode::None;

private:
	FMovieSceneEvaluationOperand Operand;
	FDreamSequenceSubtitleSectionTemplate& Template;
	FDreamSequenceSubtitleSectionContent Content;
};
