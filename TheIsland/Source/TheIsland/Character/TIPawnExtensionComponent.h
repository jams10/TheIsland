#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "TIPawnExtensionComponent.generated.h"

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

	/*
	*	UPawnComponent Interfaces
	*/
	// 컴포넌트가 액터에 부착된 이후 처음으로 호출되는 함수.
	virtual void OnRegister() final;

	/*
	  IGameFrameworkInitStateInterface
    */
    // FeatureName을 정의했다면 반드시 GetFeatureName() 오버라이딩 해주어야 InitState 시스템에 등록할 수 있음.
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
};