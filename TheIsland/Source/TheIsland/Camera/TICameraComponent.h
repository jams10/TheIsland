#pragma once

#include "Camera/CameraComponent.h"
#include "TICameraComponent.generated.h"

class UTICameraModeStack;
class UTICameraMode;
// TSubclassOf 전방선언.
template <class TClass> class TSubclassOf;

// (return type, delegate_name)
DECLARE_DELEGATE_RetVal(TSubclassOf<UTICameraMode>, FTICameraModeDeletage);

/*
*	UTICameraComponent
*	개별 카메라 시점을 구현한 CameraMode, CameraMode의 블렌딩을 처리하는 CameraModeStack을 이용하여 카메라 기능을 부여하는 컴포넌트.
*/
UCLASS()
class UTICameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UTICameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UTICameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTICameraComponent>() : nullptr); }

	/*
		CameraComponent interface
	*/
	// 컴포넌트가 부착 되었을 때 호출되는 함수.
	virtual void OnRegister() final;
	// 카메라 컴포넌트의 Tick이라 생각하면 됨. 매 Tick마다 불려서 업데이트를 진행함.
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/*
		member function
	*/
	// 실제로 CameraModeStack에 있는 현재 활성화된 CameraMode들을 업데이트하는 함수.
	void UpdateCameraModes();

	/*
		member variables
	*/
	// 카메라의 blending 기능을 지원하는 stack.
	UPROPERTY()
	TObjectPtr<UTICameraModeStack> CameraModeStack;

	// 현재 CameraMode 클래스를 가져오는 Delegate
	FTICameraModeDeletage DetermineCameraModeDelegate;
};