#pragma once

#include "ModularPlayerController.h"
#include "TIPlayerController.generated.h"

UCLASS()
class ATIPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ATIPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface
};