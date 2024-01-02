#pragma once

#include "GameFramework/PlayerState.h"
#include "TIPlayerState.generated.h"

UCLASS()
class ATIPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATIPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};