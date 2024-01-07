#pragma once

#include "GameFramework/Character.h"
#include "TICharacter.generated.h"

class UTIPawnExtensionComponent;
class UTICameraComponent;

UCLASS()
class ATICharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ATICharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* ACharacter Interface */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TI|Character")
	TObjectPtr<UTIPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LC|Character")
	TObjectPtr<UTICameraComponent> CameraComponent;
};