#pragma once

#include "ModularGameState.h"
#include "TIGameState.generated.h"
UCLASS()

class ATIGameState : public AModularGameStateBase
{
	GENERATED_BODY()
	
public:
	ATIGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface
};
