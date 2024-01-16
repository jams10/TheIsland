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
*	Ư�� ���忡 ���ؼ��� ����Ǵ� GameFeatureAction�� Base Ŭ����.
*/
UCLASS(Abstract)
class UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()
public:

	// GameFeature�� ����� �� ȣ��Ǵ� �Լ�. ���� ���带 ��ȸ�ϸ鼭 ���ڷ� ���� Context�� ������ World�� ���� AddToWorld()�� ȣ�����ְ� ����.
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

	// GameFeatureAction�� �ٽ� ������ ���� �Լ�.
	// PURE_VIRTUAL�� ����Ǿ� �־� ��ӹ޴� ��ü�� AddToWorld�� �������̵� �ؾ߸� ����� ������.
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );
};



