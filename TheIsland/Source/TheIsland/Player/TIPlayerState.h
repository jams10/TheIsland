#pragma once

#include "GameFramework/PlayerState.h"
#include "TIPlayerState.generated.h"

class UTIExperienceDefinition;
class UTIPawnData;

/*
*	ATIPlayerState
*	�÷��̾� ������ ��� �ִ� Ŭ����.
*	�÷��̾ ������ Pawn�� ���� ������ ��� �ִ� PawnData�� �����ϰ� ����.
*/
UCLASS()
class ATIPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATIPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region Actor Interface
	// ������ ������Ʈ �ʱ�ȭ �Ϸ� �� ȣ��Ǵ� �Լ�.
	virtual void PostInitializeComponents() final;
#pragma endregion

	// Experience�� �ε� �Ǿ��� �� ȣ������ �ݹ� �Լ�.	
	void OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience);

	// ���� ������� PawnData.
	UPROPERTY()
	TObjectPtr<const UTIPawnData> PawnData;
};