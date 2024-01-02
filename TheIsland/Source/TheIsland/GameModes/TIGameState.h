#pragma once

#include "GameFramework/GameStateBase.h"
#include "TIGameState.generated.h"

class UTIExperienceManagerComponent;

/*
*	ATIGameState
*	���� ������ ���� ������ �����ϰ� �ִ� Ŭ����.
*	Experience�� �����ϴ� ExperienceManagerComponent�� ��� ����.
*/
UCLASS()
class ATIGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATIGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TObjectPtr<UTIExperienceManagerComponent> ExperienceManagerComponent;
};
