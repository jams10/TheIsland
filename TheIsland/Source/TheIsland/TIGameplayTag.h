#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/*
*	FTIGameplayTags
*	native 게임 플레이 태그들을 관리하는 싱글톤 매니저.
*/
struct FTIGameplayTags
{
	static const FTIGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	/*
	* 아래의 GameplayTag는 GameFeature의 초기화 과정 단계를 의미함.
	* - GameInstance 초기화 과정에 UGameFrameworkComponentManager의 RegisterInitState로 등록되어 선형적으로(linear) 업데이트 됨.
	* - 이 초기화 GameplayTag는 게임의 Actor 사이에서 공유되며, GameFrameworkInitStateInterface를 상속받은 클래스는 초기화 상태(Init State)를 상태 머신과 같이 관리 가능한 인터페이스로 활용 가능함.
	* - 즉, GameFrameworkInitStateInterface 상속 받은 클래스는 초기화를 단계별로 설정하는 것이 가능해짐.
	*/
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

private:
	// static 변수 초기화는 .cpp에서 해주는 것을 잊지 말자!
	static FTIGameplayTags GameplayTags;
};