#pragma once

#include "GameFramework/PlayerController.h"
#include "TIPlayerController.generated.h"

UCLASS()
class ATIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATIPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};