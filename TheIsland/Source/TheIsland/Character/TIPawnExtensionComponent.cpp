#include "TIPawnExtensionComponent.h"
#include "TheIsland/TILogChannels.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPawnExtensionComponent)

const FName UTIPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UTIPawnExtensionComponent::UTIPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 이 컴포넌트 클래스는 초기화를 위해 우리가 임의적으로 트리거 시켜서 이벤트 방식으로 로직을 수행할 것이기 때문에 Tick은 필요 없으므로 꺼줌.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UTIPawnExtensionComponent::OnRegister()
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
