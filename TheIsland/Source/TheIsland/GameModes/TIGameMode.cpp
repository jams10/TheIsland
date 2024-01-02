#include "TIGameMode.h"
#include "TIGameState.h"
#include "TheIsland/Character/TICharacter.h"
#include "TheIsland/Player/TIPlayerController.h"
#include "TheIsland/Player/TIPlayerState.h"
#include "TIExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIGameMode)

ATIGameMode::ATIGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = ATIGameState::StaticClass();
	PlayerControllerClass = ATIPlayerController::StaticClass();
	PlayerStateClass = ATIPlayerState::StaticClass();
	DefaultPawnClass = ATICharacter::StaticClass();
}

void ATIGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해 초기화 작업이 진행되므로, 현 프레임에는 Lyra에 새로 도입된 Experience 초기화를 진행할 수 없음.
	// 이를 처리하기 위해, 한 프레임 뒤에 이벤트를 받아 초기화 작업을 이어 나갈 수 있도록 함.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ATIGameMode::InitGameState()
{
	Super::InitGameState();

	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// ExperienceManagerComponent 함수를 사용해 Experience가 로드 되었을 때 초기화 작업을 수행하기 위해 델리게이트를 걸어 놓음.
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ATIGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
}

void ATIGameMode::OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience)
{
}
