#pragma once

#include "TICameraMode.generated.h"

/*
*	UTICameraMode
*	개별 카메라 시점을 구현한 클래스. ex) 비조준, 조준시 시점 모두 개별 카메라 모드로 만들어 사용함.
*/
UCLASS(Abstract, NotBlueprintable)
class UTICameraMode : public UObject
{
	GENERATED_BODY()
public:
	UTICameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

/*
*	UTICameraModeStack
*	카메라 모드간에 부드럽게 전환 되도록 카메라 모드들을 관리하는 클래스.
*/
UCLASS()
class UTICameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UTICameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
		member variables
	*/
	// 생성된 camera mode를 관리.
	UPROPERTY()
	TArray<TObjectPtr<UTICameraMode>> CameraModeInstances;

	// Camera Matrix Blend 업데이트 진행 큐
	UPROPERTY()
	TArray<TObjectPtr<UTICameraMode>> CameraModeStack;
};