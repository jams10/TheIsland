#include "TIPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "TheIsland/Character/TIPawnData.h"
#include "TheIsland/GameModes/TIExperienceDefinition.h"
#include "TheIsland/GameModes/TIExperienceManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPlayerState)

ATIPlayerState::ATIPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ATIPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 액터는 항상 월드에 스폰됨. 따라서, 액터를 가져오기 위해 World에 접근해 가져옴. GameState도 AActor 클래스를 상속 받는 액터임.
	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// 우리가 사용할 PawnData는 ExperienceDefinition에 들어 있기 때문에 해당 Experience가 로드 될 때까지 알 수 없음.
	// 따라서, 델리게이트를 걸어서 로드 된 이후에 PlayerState에 있는 PawnData를 Experience에 있는 PawnData로 초기화 하는 작업을 수행할 수 있도록 함.
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ATIPlayerState::OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience)
{
}
