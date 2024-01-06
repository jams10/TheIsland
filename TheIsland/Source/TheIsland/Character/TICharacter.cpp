#include "TICharacter.h"
#include "TIPawnExtensionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TICharacter)

ATICharacter::ATICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Tick을 비활성화.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent 생성.
	PawnExtComponent = CreateDefaultSubobject<UTIPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}