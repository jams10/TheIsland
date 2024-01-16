#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "TheIsland/Input/TIMappableConfigPair.h"
#include "GameFeatureAction_AddInputConfig.generated.h"

struct FComponentRequestHandle;
class APawn;

/*
*	UGameFeatureAction_AddInputConfig
*	InputConfig�� �������� �߰��ϴ� GameFeatureAction.
*/
UCLASS()
class UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:

	// GameFeature�� ����� �� ȣ��Ǵ� �Լ�.
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	// GameFeature�� ��Ȱ��ȭ �� �� ȣ��Ǵ� �Լ�. ���� ���� �̷��� �ش� feature�� �ٽ� Ȱ��ȭ �� �� ����.
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	// GameFeatureAction�� ������ ���� �Լ�.
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
	// Context�� ���� ������.
	struct FPerContextData
	{
		// FComponentRequestHandle : Actor Ŭ������ ������Ʈ�� �߰��϶�� ��û�� ���� �ڵ�.
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		// Component�� �ٿ��� Pawn Ŭ����.
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	/* GameFeatureState�� ����� ��, �ݹ� �Լ�(Ȥ�� ExtensionEvent�� GameFrameworkComponentManager�� ���� ȣ�� ����) */
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	/* InputConfig�� EnhancedInputLocalPlayerSubsystem�� �߰� �� ���� */
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);

	/* ActiveData�� �ʱ�ȭ */
	void Reset(FPerContextData& ActiveData);

	/* world -> (extension request, pawn) �� ���忡 �� �ؼ� extension request�� ��Ī�Ǵ� pawn�� �� ������ ��� �����ϴ� lookup ���̺�. */
	// GameFeature�� Context�� ����� �Բ� ���Ǵ� Ȱ��ȭ ������.
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/* PlayerMappableInputConfig�� ��� �ִ� ��� ���� */
	UPROPERTY(EditAnywhere)
	TArray<FTIMappableConfigPair> InputConfigs;
};