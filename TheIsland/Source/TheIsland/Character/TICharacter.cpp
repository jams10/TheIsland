#include "TICharacter.h"
#include "TIPawnExtensionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TICharacter)

ATICharacter::ATICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Tick�� ��Ȱ��ȭ.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent ����.
	PawnExtComponent = CreateDefaultSubobject<UTIPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

void ATICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Controller, Pawn, PlayerState�� ��� ��ȿ�� �������� InitState ��ȯ �õ�, InitState�� ���� feature �ʱ�ȭ�� �̾� ���� �� �ֵ��� ��.
	PawnExtComponent->SetupPlayerInputComponent();
}
