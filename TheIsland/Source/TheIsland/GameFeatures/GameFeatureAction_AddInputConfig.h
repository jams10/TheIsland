#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "TheIsland/Input/TIMappableConfigPair.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

struct FComponentRequestHandle;
class APawn;

/*
*	UGameFeatureAction_AddInputConfig
*	InputConfig를 동적으로 추가하는 GameFeatureAction.
*/
UCLASS()
class UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:

	// GameFeature가 적용될 때 호출되는 함수.
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	// GameFeature가 비활성화 될 때 호출되는 함수. 머지 않은 미래에 해당 feature는 다시 활성화 될 수 있음.
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	// GameFeatureAction의 로직이 들어가는 함수.
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
	// Context에 따른 데이터.
	struct FPerContextData
	{
		// FComponentRequestHandle : Actor 클래스에 컴포넌트를 추가하라는 요청에 대한 핸들.
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		// Component를 붙여줄 Pawn 클래스.
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	/* GameFeatureState가 변경될 때, 콜백 함수(혹은 ExtensionEvent를 GameFrameworkComponentManager를 통해 호출 가능) */
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	/* InputConfig를 EnhancedInputLocalPlayerSubsystem에 추가 및 제거 */
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);

	/* ActiveData를 초기화 */
	void Reset(FPerContextData& ActiveData);

	/* world -> (extension request, pawn) 각 월드에 한 해서 extension request와 매칭되는 pawn을 한 쌍으로 묶어서 관리하는 lookup 테이블. */
	// GameFeature의 Context의 변경과 함께 사용되는 활성화 데이터.
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/* PlayerMappableInputConfig를 들고 있는 멤버 변수 */
	UPROPERTY(EditAnywhere)
	TArray<FTIMappableConfigPair> InputConfigs;
};