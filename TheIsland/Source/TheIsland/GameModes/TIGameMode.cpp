#include "TIGameMode.h"
#include "TIGameState.h"
#include "TheIsland/Character/TICharacter.h"
#include "TheIsland/Player/TIPlayerController.h"
#include "TheIsland/Player/TIPlayerState.h"
#include "TheIsland/TILogChannels.h"
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
	// 이 함수에서는 우리가 로딩할 ExperienceDefinition 애셋의 PrimaryAssetId를 가져와서, OnMatchAssignmentGiven 함수로 넘겨줌.

	FPrimaryAssetId ExperienceId; // PrimaryAssetId의 경우 애셋 타입과 이름으로 구성됨.

	// 아무런 설정도 하지 않은 경우 기본적으로 불러올 ExperienceDefinition.
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("TIExperienceDefinition"), FName("B_DefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void ATIGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 이 함수는 ExperienceManagerComponent를 활용하여 Experience를 로드를 시작함.
	check(ExperienceId.IsValid());

	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

void ATIGameMode::OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience)
{
}

APawn* ATIGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UE_LOG(LogTI, Log, TEXT("SpawnDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void ATIGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded()) // Experience가 로드 되었을 때만 HandleStartingNewPlayer() 함수를 호출하도록 함.
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

bool ATIGameMode::IsExperienceLoaded() const
{
	check(GameState);
	// ExperienceManagerComponent를 통해 Experience 로드 여부를 가져옴.
	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}
