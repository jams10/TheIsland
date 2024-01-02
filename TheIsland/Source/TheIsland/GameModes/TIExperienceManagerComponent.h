﻿#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "TIExperienceManagerComponent.generated.h"

class UTIExperienceDefinition;

enum class ETIExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating,
};

// const UTIExperienceDefinition* 타입을 넘겨주는 델리게이트 타입.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTIExperienceLoaded, const UTIExperienceDefinition*);

/*
*	UTIExperienceManagerComponent
*	게임의 현재 상태를 저장하고 있는 GameState에 컴포넌트로 추가되는 클래스.
*	Experience의 상태 정보를 가지고 있으며, Manager라는 이름에 맞게 Experience 로딩 상태 업데이트 및 이벤트를 관리함.
*/
UCLASS()
class UTIExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UTIExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 현재 Experience가 로드 되었는지 체크하는 함수.
	bool IsExperienceLoaded() { return  (LoadState == ETIExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	// 이미 Experience 로딩이 끝났다면 인자로 넘겨준 Delegate를 로드된 Experience와 함께 호출하고,
	// Experience가 로드 되지 않았다면, OnExperienceLoaded에 델리게이트를 바인딩 해주는 함수.
	void CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate&& Delegate);

	// 현재 적용된 Experience. 
	// TODO : 멀티플레이어를 고려할 경우, Lyra에서 처럼 replicated 변수로 만들어 줘야 함.
	UPROPERTY()
	TObjectPtr<const UTIExperienceDefinition> CurrentExperience;

	// Experience의 로딩 상태 저장하는 멤버 변수.
	ETIExperienceLoadState LoadState = ETIExperienceLoadState::Unloaded;

	// Experience 로딩이 완료된 이후 호출해줄 Delegate.
	FOnTIExperienceLoaded OnExperienceLoaded;
};