#pragma once

#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.h"
#include "TIPlayerController.generated.h"

UCLASS()
class ATIPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ATIPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};