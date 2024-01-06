#pragma once

#include "GameFramework/Character.h"
#include "TICharacter.generated.h"

class UTIPawnExtensionComponent;

UCLASS()
class ATICharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ATICharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TI|Character")
	TObjectPtr<UTIPawnExtensionComponent> PawnExtComponent;
};