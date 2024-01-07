#include "TICameraComponent.h"
#include "TICameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TICameraComponent)

UTICameraComponent::UTICameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CameraModeStack(nullptr)
{

}

void UTICameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeStack)
	{
		// 초기화 (BeginPlay와 같은)가 딱히 필요 없는 객체로, NewObject로 할당.
		CameraModeStack = NewObject<UTICameraModeStack>(this);
	}
}

void UTICameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate에 콜백 함수가 바인딩 되어 있다면 호출해 CameraMode 클래스를 얻어옴.
	// 현재 플레이어의 카메라, 입력을 설정하는 HeroComponent 쪽에서 DetermineCameraModeDelegate에 콜백 함수를 바인딩 해주고 있음.
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (const TSubclassOf<UTICameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			// CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}

void UTICameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	UpdateCameraModes();
}