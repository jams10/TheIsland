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
		// �ʱ�ȭ (BeginPlay�� ����)�� ���� �ʿ� ���� ��ü��, NewObject�� �Ҵ�.
		CameraModeStack = NewObject<UTICameraModeStack>(this);
	}
}

void UTICameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DetermineCameraModeDelegate�� �ݹ� �Լ��� ���ε� �Ǿ� �ִٸ� ȣ���� CameraMode Ŭ������ ����.
	// ���� �÷��̾��� ī�޶�, �Է��� �����ϴ� HeroComponent �ʿ��� DetermineCameraModeDelegate�� �ݹ� �Լ��� ���ε� ���ְ� ����.
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

	// PushCameraMode() �Լ� ȣ���� ���� ���� Ȱ��ȭ�� CameraMode�� CameraModeStack�� �ֻ������ �Ű���.
	UpdateCameraModes();

	// CameraModeStack�� �ִ� CameraMode�� BlendWeight�� ���� ����, ���� ����� ���� ī�޶� ������ �����.
	FTICameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView); // ���� ��� ����� CameraModeView�� �޾���. 

	// Keep player controller in sync with the latest view.
    // ��Ʈ�ѷ��� ȸ�� ���� �ֽ� View ����� ������.
	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Apply any offset that was added to the field of view.
    // FOV ������ ���� ��������.
	CameraModeView.FieldOfView += FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	// Keep camera component in sync with the latest view.
    // ī�޶� ������Ʈ�� Ʈ������ ���� �ֽ� View ����� ������.
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