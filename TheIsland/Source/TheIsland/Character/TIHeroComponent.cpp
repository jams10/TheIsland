#include "TIHeroComponent.h"
#include "TheIsland/TIGameplayTag.h"
#include "TheIsland/TILogChannels.h"
#include "TIPawnExtensionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIHeroComponent)

const FName UTIHeroComponent::NAME_ActorFeatureName("Hero");

UTIHeroComponent::UTIHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 이 컴포넌트 클래스는 초기화를 위해 우리가 임의적으로 트리거 시켜서 이벤트 방식으로 로직을 수행할 것이기 때문에 Tick은 필요 없으므로 꺼줌.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UTIHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록 되었는지 확인.
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogTI, Error, TEXT("this component has been added to BP whose class is not a Pawn!"));
		}
	}

	// IGameFrameworkInitStateInterface 클래스 인터페이스 함수를 사용해 GameFrameworkComponentManager에 InitState 사용을 위해 등록!
	// 이 함수를 호출하면, InitState의 초기 값은 NAME_None.
	RegisterInitStateFeature();
}

void UTIHeroComponent::BeginPlay()
{
	// BeginPlay에서 본격적인 InitState의 변화가 시작됨. 
	Super::BeginPlay();

	// PawnExtension feature에 대해 InitState가 변경될 때 마다 OnActorInitStateChanged() 함수가 호출되도록 델리게이트를 바인딩. 
	BindOnActorInitStateChanged(UTIPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// 나(PawnExtensionComponent)의 InitState를 InitState_Spawned 상태로 변환 시도.
	ensure(TryToChangeInitState(FTIGameplayTags::Get().InitState_Spawned));

	// 다음 InitState로 변환. 강제로 상태를 업데이트.
	CheckDefaultInitialization();
}

void UTIHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister의 RegisterInitStateFeature()를 통해 InitState 시스템에 feature를 등록해 주었다면,
	// 여기서는 UnregisterInitStateFeature() 함수를 호출해서 InitState 시스템에서 feature를 등록 해제 해줌.
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}
