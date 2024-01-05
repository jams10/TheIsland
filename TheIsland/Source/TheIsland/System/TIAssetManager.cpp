
#include "TIAssetManager.h"
#include "TheIsland/TIGameplayTag.h"
#include "TheIsland/TILogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIAssetManager)

UTIAssetManager::UTIAssetManager()
	:Super()
{

}

UTIAssetManager& UTIAssetManager::Get()
{
	check(GEngine);

	// 우리는 AssetManager를 UEngine의 GEngine의 AssetManager의 클래스를 오버라이드 했기 때문에, GEngine에 우리의 애셋 매니저가 있음.
	if (UTIAssetManager* Singleton = Cast<UTIAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTI, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to TIAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UTIAssetManager>();
}

bool UTIAssetManager::ShouldLogAssetLoads()
{
	// FParse::Param과 FCommandLine::Get()을 통해, 우리가 원하는 속성 값을 CommandLine으로 가져올 수 있음.
	// 우리가 애셋을 로드할 때 그 시간을 로깅하고 싶다면, 실행 시킬 때 'LogAssetLoads' 명령어 인자를 추가로 넣어주면 됨.
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* UTIAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 이 함수를 만들어준 이유는 버벅 거림을 유발할 수 있는 정적 로딩이 있는지 체크하기 위함. 

	if (AssetPath.IsValid())
	{
		// FScopeLogTime을 통해 애셋 로드에 얼만큼의 시간이 걸렸는지 확인할 수 있음.
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 초단위로 로깅을 진행함.
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 여기서 두 가지의 분기가 있음.
		// 1. 애셋 매니저가 유효한 경우, AssetManager의 StreamableManager를 통해 정적 로딩을 수행.
		// 2. 애셋 매니저가 유효하지 않은 경우, FSoftObjectPath를 통해 바로 정적 로딩을 수행.
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UTIAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		// thread-safe 보장을 위해 lock을 걸고 LoadedAssets 컨테이너에 로드된 애셋을 추가해줌.
		// 애셋 매니저가 에디터 초기화 단계에서 호출되는데, 이 단계에서는 스레드 안정성이 보장되지 않기 때문.
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

void UTIAssetManager::StartInitialLoading()
{
	// 가상 함수 오버라이딩 이므로 빠지지 말고 해주자!
	Super::StartInitialLoading();

	UE_LOG(LogTI, Display, TEXT("UTIAssetManager::StartInitialLoading"));

	// TIGameplayTags 초기화.
	// Lyra에서는 STARTUP_JOB() 매크로를 활용하고 있으나, 현재 우리는 로딩과 관계 없으므로 간단하게 구현함.
	FTIGameplayTags::InitializeNativeTags();
}
