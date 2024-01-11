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
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}

void UTICameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	// PushCameraMode() 함수 호출을 통해 현재 활성화된 CameraMode를 CameraModeStack의 최상단으로 옮겨줌.
	UpdateCameraModes();

	// CameraModeStack에 있는 CameraMode를 BlendWeight를 통해 블렌딩, 블렌딩 결과를 통해 카메라 시점을 계산함.
	FTICameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView); // 시점 계산 결과를 CameraModeView로 받아줌. 

	// Keep player controller in sync with the latest view.
    // 컨트롤러의 회전 값을 최신 View 결과와 맞춰줌.
	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Apply any offset that was added to the field of view.
    // FOV 오프셋 값을 적용해줌.
	CameraModeView.FieldOfView += FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	// Keep camera component in sync with the latest view.
    // 카메라 컴포넌트의 트랜스폼 값을 최신 View 결과와 맞춰줌.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	FieldOfView = CameraModeView.FieldOfView;

	// Fill in desired view.
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	// See if the CameraActor wants to override the PostProcess settings used.
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}

	//if (IsXRHeadTrackedCamera())
	//{
	//	// In XR much of the camera behavior above is irrellevant, but the post process settings are not.
	//	Super::GetCameraView(DeltaTime, DesiredView);
	//}
}