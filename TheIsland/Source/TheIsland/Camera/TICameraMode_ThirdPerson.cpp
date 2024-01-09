#include "TICameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TICameraMode_ThirdPerson)

UTICameraMode_ThirdPerson::UTICameraMode_ThirdPerson()
{
	TargetOffsetCurve = nullptr;
}

void UTICameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	// Base class인 CameraMode의 UpdateView와 동일함.
	// 이 코드에 추가적으로 우리가 각 특정 모드에 따라 구현해야 할 사항들을 추가해주면 됨.
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// TargetOffsetCurve가 override 되어 있따면, Curve에 있는 값을 가져와서 적용함.
	// - Camera 관점에서 Character의 어느 부분에 Target으로 할지 결정하는 것으로 이해하면 됨.
	if (TargetOffsetCurve)
	{
		// PivotRotation의 Pitch 값을 입력으로 해서 Curve 값을 받아옴.
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
