#pragma once

#include "GameFramework/Character.h"
#include "TICharacter.generated.h"

UCLASS()
class ATICharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ATICharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};