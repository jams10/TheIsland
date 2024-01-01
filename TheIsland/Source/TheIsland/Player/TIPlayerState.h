#pragma once

#include "ModularPlayerState.h"
#include "TIPlayerState.generated.h"

UCLASS()
class ATIPlayerState : public AModularPlayerState
{
	GENERATED_BODY()
	
public:
	ATIPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	//~End of APlayerState interface
};