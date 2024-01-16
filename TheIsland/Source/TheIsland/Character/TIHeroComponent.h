#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "TIHeroComponent.generated.h"

struct FInputActionValue;
struct FTIMappableConfigPair;
class UTICameraMode; 
class UInputComponent;

/*
*	TIHeroComponent
*	플레이어가 제어하는 폰(또는 플레이어를 시뮬레이션하는 봇)에 대한 입력 및 카메라 처리를 설정하는 컴포넌트.
*   초기화를 조정하기 위해 PawnExtensionComponent에 의존함.
*/
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent)) // 블루프린트에서 컴포넌트로 추가하기 위해 'BlueprintSpawnableComponent' 파라미터를 추가로 설정.
class UTIHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UTIHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	/* Extension Event 이름 정의 */
	static const FName NAME_BindInputsNow;

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
	// InitState가 변경 되었을 때 같이 호출되는 함수. InitState 변경 시 처리해줄 작업이 처리되는 곳임. HeroComponent에서는 입력과 카메라 관련 초기화를 수행함.
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;

protected:
	// CameraComponent에서 사용할 CameraMode 클래스를 리턴 해주는 함수.
	TSubclassOf<UTICameraMode> DetermineCameraMode() const;

	// 입력 초기화 함수. InputComponent의 함수들을 이용하여 InputAction과 콜백 함수들을 바인딩 해줌.
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	/* InputAction에 바인딩 해줄 콜백 함수들. */ 
	// 플레이어 이동 처리 함수.
	void Input_Move(const FInputActionValue& InputActionValue);
	// 플레이어 시점(카메라) 이동 처리 함수.
	void Input_LookMouse(const FInputActionValue& InputActionValue);

protected:

	// Ability에 의해 세팅된 CameraMode.
	UPROPERTY()
	TSubclassOf<UTICameraMode> AbilityCameraMode;

	// 내부에 PlayerMappableInputConfig를 들고 있는 구조체.
	UPROPERTY(EditAnywhere)
	TArray<FTIMappableConfigPair> DefaultInputConfigs;
};