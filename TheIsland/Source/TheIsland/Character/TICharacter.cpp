#include "TICharacter.h"
#include "TIPawnExtensionComponent.h"
#include "TheIsland/Camera/TICameraComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TICharacter)

ATICharacter::ATICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Tick�� ��Ȱ��ȭ.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent ����.
	PawnExtComponent = CreateDefaultSubobject<UTIPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent ����.
	CameraComponent = CreateDefaultSubobject<UTICameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

void ATICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Controller, Pawn, PlayerState�� ��� ��ȿ�� �������� InitState ��ȯ �õ�, InitState�� ���� feature �ʱ�ȭ�� �̾� ���� �� �ֵ��� ��.
	PawnExtComponent->SetupPlayerInputComponent();
}
