#pragma once

#include "GameFramework/GameStateBase.h"
#include "TIGameState.generated.h"

class UTIExperienceManagerComponent;

/*
*	ATIGameState
*	현재 게임의 상태 정보를 저장하고 있는 클래스.
*	Experience를 관리하는 ExperienceManagerComponent를 들고 있음.
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
