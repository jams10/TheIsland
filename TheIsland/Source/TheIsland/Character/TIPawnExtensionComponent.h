#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "TIPawnExtensionComponent.generated.h"

class UTIPawnData;

/*
*	TIPawnExtensionComponent
*	PawnExtensionComponent의 경우 Pawn에 추가하는 다른 컴포넌트들의 초기화 상태를 조율하는 매니저 같은 역할을 함.
*/
UCLASS()
class UTIPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UTIPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	static UTIPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTIPawnExtensionComponent>() : nullptr); }

	/*
	*	Member functions
	*/
	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const UTIPawnData* InPawnData);
	// Character 클래스의 SetupPlayerInputComponent() 함수에 의해 호출되는 함수. 조건에 따라 중단된 InitState 단계를 계속 이어나가기 위해 호출.
	void SetupPlayerInputComponent();

	/*
	*	UPawnComponent Interfaces
	*/
	// 컴포넌트가 액터에 부착된 이후 처음으로 호출되는 함수.
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;


	/*
	  IGameFrameworkInitStateInterface
    */
    // FeatureName을 정의했다면 반드시 GetFeatureName() 오버라이딩 해주어야 InitState 시스템에 등록할 수 있음.
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	// 관찰하는 feature의 특정 InitState가 변경 되었을 때 호출되는 함수.
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	// InitState 변경이 가능한지 여부를 체크하는 함수.
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	// 강제로 InitState 단계를 따라 InitState 변경이 가능한지 체크 후 InitState를 업데이트 하는 함수.
	virtual void CheckDefaultInitialization() final;

	/*
		Pawn을 생성한 데이터를 캐싱.
	*/
	UPROPERTY(EditInstanceOnly, Category = "TI|Pawn")
	TObjectPtr<const UTIPawnData> PawnData;
};