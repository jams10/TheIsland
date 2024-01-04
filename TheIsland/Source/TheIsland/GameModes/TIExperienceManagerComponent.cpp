#include "TIExperienceManagerComponent.h"
#include "GameFeaturesSubsystemSettings.h"
#include "TIExperienceDefinition.h"
#include "TheIsland/System/TIAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIExperienceManagerComponent)

UTIExperienceManagerComponent::UTIExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTIExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		// Experience가 이미 로드 되어 있다면 현재 로드된 Experience를 전달하면서 인자로 받아준 Delegate를 그대로 실행.
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		// OnExperienceLoaded multicast 델리게이트의 호출 리스트에 Delegate 인스턴스를 추가하여, Experience가 로드 되었을 때 응답 받을 수 있도록 함.
		OnExperienceLoaded.Add(MoveTemp(Delegate)); // Delegate 인스턴스 복사 비용을 낮추기 위해 Move를 통해 인스턴스 전달.
	}
}

void UTIExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UTIAssetManager& AssetManager = UTIAssetManager::Get();

	// ExperienceId에 해당하는 ExperienceDefinition 블루프린트 클래스를 로드함.
	TSubclassOf<UTIExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// 로드한 ExperienceDefinition 블루프린트 클래스의 CDO를 얻어와 CurrentExperience로 설정.
	const UTIExperienceDefinition* Experience = GetDefault<UTIExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// 현재 experience로 세팅.
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UTIExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ETIExperienceLoadState::Unloaded);

	// Experience 로딩 상태를 Loading으로 설정.
	LoadState = ETIExperienceLoadState::Loading;

	UTIAssetManager& AssetManager = UTIAssetManager::Get();

	// 비동기로 로드할 PrimaryAsset의 id들이 담긴 set.
	TSet<FPrimaryAssetId> BundleAssetList;

	// CDO를 사용하여 비동기 로드할 Experience의 정확한 id를 얻어오고, BundleList에 추가해줌.
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load assets associated with the experience.
	// 아래는 우리가 후일 GameFeature를 사용하여 Experience에 바인딩 된 GameFeature Plugin을 로딩할 Bundle 이름을 추가해줌.
	// bundle의 경우, 우리가 로딩할 애셋의 카테고리 이름이라고 생각하면 됨.
	TArray<FName> BundleToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가됨. 즉, 싱글 게임이어도 client, server 둘다 로딩에 추가됨.
		// Standalone을 Client + Server라고 생각하면 됨. 
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundleToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundleToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	// Experience 로드가 끝나면 OnExperienceLoadComplete() 함수를 호출하는 Delegate 생성.
	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// BundleAssetList에 넣어준 Experience 블루프린트 애셋을 비동기 로드함.
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundleToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료 되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자! -> OnExperienceLoadComplete() 호출.
		// 함수 내부에서 FStreamableDelegateDelayHelper 클래스 객체를 만들고, 여기에 delegate를 추가해서 호출하게 됨. 
		// FStreamableDelegateDelayHelper의 경우, FTickableGameObject 클래스를 상속 받는데, FTickableGameObject는 보통 월드 Tick 호출이 끝난 뒤에 호출됨. 
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		// 로딩이 완료 되지 않았다면 Handle에 로드가 완료, 취소 될 때 호출할 Delegate를 바인딩 해줌. 
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}
}

void UTIExperienceManagerComponent::OnExperienceLoadComplete()
{
	// 이 함수는 FStreamableHandle::ExecuteDelegate() 함수 내부에서 생성한 FStreamableDelegateDelayHelper 인스턴스에 의해 호출됨.

	OnExperienceFullLoadCompleted();
}

void UTIExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ETIExperienceLoadState::Loaded);

	// Experience가 로드 되었으므로 로딩 상태를 Loaded로 바꿔주고, OnExperienceLoaded 멀티 캐스트 델리게이트에 등록된 델리게이트들에 모두 broadcast.
	LoadState = ETIExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}
