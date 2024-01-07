#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "TIHeroComponent.generated.h"

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
};