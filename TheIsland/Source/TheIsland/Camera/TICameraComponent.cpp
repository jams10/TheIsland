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
			// CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}

void UTICameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	UpdateCameraModes();
}