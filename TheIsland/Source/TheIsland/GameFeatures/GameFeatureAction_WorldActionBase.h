#pragma once

#include "Containers/Map.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction_WorldActionBase.generated.h"

class FDelegateHandle;
class UGameInstance;
struct FGameFeatureActivatingContext;
struct FGameFeatureDeactivatingContext;
struct FWorldContext;

/*
*	UGameFeatureAction_WorldActionBase
*	특정 월드에 대해서만 적용되는 GameFeatureAction의 Base 클래스.
*/
UCLASS(Abstract)
class UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()
public:

	// GameFeature가 적용될 때 호출되는 함수. 지금 월드를 순회하면서 인자로 들어온 Context와 동일한 World에 대해 AddToWorld()를 호출해주고 있음.
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

	// GameFeatureAction의 핵심 로직이 들어가는 함수.
	// PURE_VIRTUAL로 선언되어 있어 상속받는 객체가 AddToWorld를 오버라이드 해야만 사용이 가능함.
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );
};



