#pragma once

#include "GameFramework/GameStateBase.h"
#include "TIGameState.generated.h"
UCLASS()

class ATIGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATIGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
