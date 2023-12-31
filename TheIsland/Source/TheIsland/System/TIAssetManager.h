#pragma once

#include "Engine/AssetManager.h"
#include "TIAssetManager.generated.h"

UCLASS()
class UTIAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UTIAssetManager();

	// 싱글톤 인스턴스를 리턴 하는 함수.
	static UTIAssetManager& Get();

	// TSoftObjectPtr에 의해 참조되는 애셋을 리턴함. 애셋이 미리 로드 되어 있지 않은 경우, 동기적으로 애셋을 로드함.
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// TSoftObjectPtr에 의해 참조되는 서브 클래스를 리턴함. 애셋이 미리 로드 되어 있지 않은 경우, 동기적으로 애셋을 로드함.
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

protected:
	// 엔진 초기화 시점에 초기화 되어야 할 것들, 불러와야 할 것들을 처리하는 함수. ex) 게임 태그, 게임 큐 매니저 등등.
	virtual void StartInitialLoading() override;

	// 애셋을 로드 시간을 출력하는지 유무를 명령어 인자를 받아서 결정하는 함수.
	static bool ShouldLogAssetLoads();

	// 로드한 애셋을 캐싱하는 함수.
	void AddLoadedAsset(const UObject* Asset);

	// 버벅 거림을 유발할 수 있는 정적 로딩이 있는지 체크하기 위해 만들어준 함수.
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

private:

	// 애셋 매니저에 의해 로드되고 추적되는 애셋들. 
	UPROPERTY() // GC의 대상이 됨을 알 수 있음.
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// 로드된 애셋의 리스트를 조작할 때(modify) scope lock을 위해 사용됨. Object 단위의 locking.
	FCriticalSection LoadedAssetsCritical;
};

template<typename AssetType>
AssetType* UTIAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid()) // 로드해서 가져오려는 애셋이 유효한지 확인.
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset) // 애셋을 로드하지 않았다면 로드해줌.
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory) // bKeepInMemory = true인 경우, 애셋을 메모리에 캐싱하도록 함.
		{
			// AddLoadAsset의 경우 메모리에 애셋을 상주 하도록 만들기 위한 장치라고 생각하면 됨.
			// 한 번 애셋이 등록될 경우, 직접 내리지 않는 한 메모리에서 unload 되지 않음. (애셋을 캐싱)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
TSubclassOf<AssetType> UTIAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}