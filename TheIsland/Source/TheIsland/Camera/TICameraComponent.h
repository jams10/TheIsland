#pragma once

#include "Camera/CameraComponent.h"
#include "TICameraComponent.generated.h"

class UTICameraModeStack;
class UTICameraMode;
// TSubclassOf ���漱��.
template <class TClass> class TSubclassOf;

// (return type, delegate_name)
DECLARE_DELEGATE_RetVal(TSubclassOf<UTICameraMode>, FTICameraModeDeletage);

/*
*	UTICameraComponent
*	���� ī�޶� ������ ������ CameraMode, CameraMode�� ������ ó���ϴ� CameraModeStack�� �̿��Ͽ� ī�޶� ����� �ο��ϴ� ������Ʈ.
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
	// ������Ʈ�� ���� �Ǿ��� �� ȣ��Ǵ� �Լ�.
	virtual void OnRegister() final;
	// ī�޶� ������Ʈ�� Tick�̶� �����ϸ� ��. �� Tick���� �ҷ��� ������Ʈ�� ������.
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/*
		member function
	*/
	// ������ CameraModeStack�� �ִ� ���� Ȱ��ȭ�� CameraMode���� ������Ʈ�ϴ� �Լ�.
	void UpdateCameraModes();
	// ī�޶� �����ִ� ����� TargetActor�� �����ϴ� �Լ�. ī�޶� ������ TargetActor�� ������� ��ġ�� ȸ�� ���� ����.
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	// ���� CameraMode Ŭ������ �������� Delegate
	FTICameraModeDeletage DetermineCameraModeDelegate;

protected:
	// ī�޶��� blending ����� �����ϴ� stack.
	UPROPERTY()
	TObjectPtr<UTICameraModeStack> CameraModeStack;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;
};