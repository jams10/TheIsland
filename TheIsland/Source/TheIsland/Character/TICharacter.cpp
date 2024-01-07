#include "TICharacter.h"
#include "TIPawnExtensionComponent.h"
#include "TheIsland/Camera/TICameraComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TICharacter)

ATICharacter::ATICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Tick을 비활성화.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent 생성.
	PawnExtComponent = CreateDefaultSubobject<UTIPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent 생성.
	CameraComponent = CreateDefaultSubobject<UTICameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

void ATICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Controller, Pawn, PlayerState가 모두 유효한 시점에서 InitState 전환 시도, InitState를 통한 feature 초기화를 이어 나갈 수 있도록 함.
	PawnExtComponent->SetupPlayerInputComponent();
}
