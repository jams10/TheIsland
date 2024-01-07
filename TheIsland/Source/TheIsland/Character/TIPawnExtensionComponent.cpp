#include "TIPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"
#include "TheIsland/Character/TIPawnData.h"
#include "TheIsland/TIGameplayTag.h"
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

void UTIPawnExtensionComponent::SetPawnData(const UTIPawnData* InPawnData)
{
	// Pawn에 대해 Authority가 없을 경우, SetPawnData는 진행하지 않음.
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	// PawnData 업데이트
	PawnData = InPawnData;

	// 라이라에는 아래 함수를 호출해주는데, 순서도를 따라가다 보면 호출할 필요가 전혀 없는 함수임.
	// CheckDefaultInitialization();
}

void UTIPawnExtensionComponent::SetupPlayerInputComponent()
{
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

void UTIPawnExtensionComponent::BeginPlay()
{
	// BeginPlay에서 본격적인 InitState의 변화가 시작됨. 
	Super::BeginPlay();

	// 모든 feature에 대해 InitState가 변경될 때 마다 OnActorInitStateChanged() 함수가 호출되도록 델리게이트를 바인딩. 
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// 나(PawnExtensionComponent)의 InitState를 InitState_Spawned 상태로 변환 시도.
	ensure(TryToChangeInitState(FTIGameplayTags::Get().InitState_Spawned));

	// 다음 InitState로 변환. 강제로 상태를 업데이트.
	CheckDefaultInitialization();
}

void UTIPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister의 RegisterInitStateFeature()를 통해 InitState 시스템에 feature를 등록해 주었다면,
	// 여기서는 UnregisterInitStateFeature() 함수를 호출해서 InitState 시스템에서 feature를 등록 해제 해줌.
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UTIPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// 자기 자신을 제외한 다른 feature component가 변경 되었는지를 체크함. 
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// LCPawnExtensionComponent는 다른 Feature Component들의 상태가 DataAvailable 임을 관찰하여, Sync를 맞추는 구간이 있었다.(CanChangeInitState)
		// 이를 가능하게 만들고자 OnActorInitStateChanged에서는 DataAvailable에 대해 지속적으로 CheckDefaultInitialization을 호출하여, 상태를 확인함.
		const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UTIPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();

	// None -> Spawned 상태로 전환 가능한지 체크.
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// PawnExtensionComponent가 Pawn에 잘 부착 되었다면, Spawned 상태로 전환 가능.
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// PawnData가 설정 되어 있고, Controller가 세팅 되어 있으면 전환 가능.
		if (!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority(); // 싱글에서는 무조건 true 리턴.
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bHasAuthority || bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// 자기 자신도 DataAvailable, 나머지 feature component들도 DataAvailable 상태일 때 전환 가능.
		// HaveAllFeaturesReachedInitState() 함수를 통해 다른 feature들도 DataAvailable 상태로 도달 했는지 체크함.
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// 현재 상태가 DataInitialized이면 전환 가능.
		return true;
	}

	// 우리가 정의해준 InitState 순서가 아니면, 조건에 해당 되지 않으면 false를 리턴.
	return false;
}

void UTIPawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent는 Feature Component들의 초기화를 관장하는 Component이다.
    // 다른 feature들의 CheckDefaultInitialization() 함수를 호출, 다른 feature들의 InitState를 강제로 업데이트 시켜줌. 
	CheckDefaultInitializationForImplementers();

	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();

	// 사용자 정의 InitState를 직접 넘겨줘야 한다...
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// InitState 단계에 따라 강제로 InitState 
	ContinueInitStateChain(StateChain);
}
