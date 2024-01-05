#include "TIGameInstance.h"
#include "TheIsland/TIGameplayTag.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIGameInstance)

UTIGameInstance::UTIGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTIGameInstance::Init()
{
	Super::Init();

	// InitState를 나타내는 게임 플레이 태그를 통해 InitState를 등록해줌.
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FTIGameplayTags& GameplayTags = FTIGameplayTags::Get();

		// 선형적인 구조로 InitState를 등록 시킬 수 있음. 호출 순서대로 등록됨.
		// 함수의 첫 번째 인자가 등록할 새 State, 세 번째 인자가 새 State 이전의 State임.
		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void UTIGameInstance::Shutdown()
{
	Super::Shutdown();
}