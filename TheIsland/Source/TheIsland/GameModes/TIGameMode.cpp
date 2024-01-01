#include "TIGameMode.h"
#include "TIGameState.h"
#include "TheIsland/Character/TICharacter.h"
#include "TheIsland/Player/TIPlayerController.h"
#include "TheIsland/Player/TIPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIGameMode)

ATIGameMode::ATIGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = ATIGameState::StaticClass();
	PlayerControllerClass = ATIPlayerController::StaticClass();
	PlayerStateClass = ATIPlayerState::StaticClass();
	DefaultPawnClass = ATICharacter::StaticClass();
}