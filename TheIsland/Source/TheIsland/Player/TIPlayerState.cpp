#include "TIPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "TheIsland/Character/TIPawnData.h"
#include "TheIsland/GameModes/TIExperienceDefinition.h"
#include "TheIsland/GameModes/TIExperienceManagerComponent.h"
#include "TheIsland/GameModes/TIGameMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPlayerState)

ATIPlayerState::ATIPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ATIPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ���ʹ� �׻� ���忡 ������. ����, ���͸� �������� ���� World�� ������ ������. GameState�� AActor Ŭ������ ��� �޴� ������.
	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// �츮�� ����� PawnData�� ExperienceDefinition�� ��� �ֱ� ������ �ش� Experience�� �ε� �� ������ �� �� ����.
	// ����, ��������Ʈ�� �ɾ �ε� �� ���Ŀ� PlayerState�� �ִ� PawnData�� Experience�� �ִ� PawnData�� �ʱ�ȭ �ϴ� �۾��� ������ �� �ֵ��� ��.
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ATIPlayerState::OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience)
{
	if (ATIGameMode* GameMode = GetWorld()->GetAuthGameMode<ATIGameMode>())
	{
		// GetPawnDataForController() �Լ������� PlayerState�� PawnData�� ���� �Ǿ� ������ �׳� PlayerState�� PawnData�� ����������,
		// PawnData�� ���� �Ǿ� ���� ������ ���� �ε��� Experience�� �ִ� PawnData�� ������ ���� ����.
		// ���� GetPawnDataForController() �Լ��� ���� ���� �ε�� Experience�� PawnData�� �����ͼ� PlayerState�� PawnData�� ��������.
		const UTIPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ATIPlayerState::SetPawnData(const UTIPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData�� �� �� ���� �Ǵ� ���� ������ ����!
	check(!PawnData);

	PawnData = InPawnData;
}
