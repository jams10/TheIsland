#include "TIHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "TheIsland/Camera/TICameraMode.h"
#include "TheIsland/Camera/TICameraComponent.h"
#include "TheIsland/Character/TIPawnData.h"
#include "TheIsland/Player/TIPlayerState.h"
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

void UTIHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
	// 바뀐 feature가 PawnExtension인지 확인.
	if (Params.FeatureName == UTIPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// PawnExtensionComponent의 상태가 DataInitialized인지 체크 후, DataInitialized이면 다음 상태로 전환 될 수 있도록 CheckDefaultInitialization() 함수 호출.
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UTIHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ATIPlayerState* PS = GetPlayerState<ATIPlayerState>();

	// None -> Spawned 상태로 전환 가능한지 체크.
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn이 유효한 경우 전환 가능.
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// PlayerState가 유효한 경우 전환 가능.
		if (!PS)
		{
			return false;
		}
		return true;
	}

	// DataAvailable -> DataInitialized 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// 즉, 모든 feature들이 DataAvailable이 되어 PawnExtensionComponent의 상태가 DataInitialized가 되었을 때 비로소 HeroComponent도 DataInitialized 상태로 전환.
		return PS && Manager->HasFeatureReachedInitState(Pawn, UTIPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// 현재 상태가 DataInitialized인 경우에 전환 가능.
		return true;
	}

	return false;
}

void UTIHeroComponent::CheckDefaultInitialization()
{
	// 그냥 ContinueInitStateChain() 함수를 호출해서 다음 InitState로 변경 가능한 경우 변경.
	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

void UTIHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FTIGameplayTags InitTags = FTIGameplayTags::Get();
	// 현재 상태가 DataAvailable이고, 전환 하고자 하는 상태가 DataInitialized이면, 작업을 처리함.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ATIPlayerState* PS = GetPlayerState<ATIPlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UTIPawnData* PawnData = nullptr;
		if (UTIPawnExtensionComponent* PawnExtComp = UTIPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UTIPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 Character에 Attach된 CameraComponent를 찾음.
			if (UTICameraComponent* CameraComponent = UTICameraComponent::FindCameraComponent(Pawn))
			{
				// CameraComponent의 delegate에 CameraMode 클래스를 리턴하는 멤버 함수 바인딩.
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

TSubclassOf<UTICameraMode> UTIHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	// PawnExtensionComponent에서 PawnData를 PlayerState에서 가져와 들고 있으므로, PawnExtensionComponent에 접근해 PawnData 가져옴.
	if (UTIPawnExtensionComponent* PawnExtComp = UTIPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UTIPawnData* PawnData = PawnExtComp->GetPawnData<UTIPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
