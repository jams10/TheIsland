#pragma once

#include "ModularCharacter.h"
#include "TICharacter.generated.h"

UCLASS()
class ATICharacter : public AModularCharacter
{
	GENERATED_BODY()
public:
	ATICharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface
};