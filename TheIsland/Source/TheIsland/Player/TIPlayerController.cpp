#include "TIPlayerController.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPlayerController)

ATIPlayerController::ATIPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ATIPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ATIPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATIPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ATIPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}
