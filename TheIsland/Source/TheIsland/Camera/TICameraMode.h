#pragma once

#include "GameplayTagContainer.h"
#include "TICameraMode.generated.h"

class UTICameraComponent;

/*
* 카메라 블렌딩 함수 타입 enum.
*/
UENUM(BlueprintType)
enum class ETICameraModeBlendFunction : uint8
{
	// Does a simple linear interpolation.
	Linear,

	// Immediately accelerates, but smoothly decelerates into the target.  Ease amount controlled by the exponent.
	EaseIn,

	// Smoothly accelerates, but does not decelerate into the target.  Ease amount controlled by the exponent.
	EaseOut,

	// Smoothly accelerates and decelerates.  Ease amount controlled by the exponent.
	EaseInOut,

	COUNT	UMETA(Hidden)
};

/*
* 카메라 시점 정보를 나타내는 구조체.
*/
struct FTICameraModeView
{
public:

	FTICameraModeView();

	// CameraMode의 BlendWeight를 바탕으로 다른 CameraMode와 함께 블렌딩. Location, Rotation, ControlRotation, FieldOfView 값이 블렌드 됨. 
	void Blend(const FTICameraModeView& Other, float OtherWeight);

public:

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/*
* 카메라 블렌딩 대상이 되는 개별 카메라 모드를 나타내는 클래스.
* 내부에 시점 정보를 나타내는 CameraView 타입 변수를 가지고 있음.
*/
UCLASS(Abstract, NotBlueprintable)
class THEISLAND_API UTICameraMode : public UObject
{
	GENERATED_BODY()

public:

	UTICameraMode();

	// 카메라 컴포넌트 얻어오는 함수.
	UTICameraComponent* GetTICameraComponent() const;
	// 월드 얻어오는 함수.
	virtual UWorld* GetWorld() const override;
	// 카메라가 바라보는 Actor를 얻어오는 함수. 
	AActor* GetTargetActor() const;
	// 카메라 모드를 통해 생성된 시점 결과인 CameraModeView를 리턴하는 함수.
	const FTICameraModeView& GetCameraModeView() const { return View; }

	// CameraModeStack에 있는 camera mode가 활성화 되면 호출되는 함수. 
	virtual void OnActivation() {};

	// CameraModeStack에 있는 camera mode가 비활성화 되면 호출되는 함수. 
	virtual void OnDeactivation() {};

	// UpdateView() : 캐릭터 위치, 컨트롤러 회전을 바탕으로 View 업데이트 + UpdateBlending() : DeltaTime, BlendAlpha 값을 바탕으로 BlendWeight 업데이트.
	void UpdateCameraMode(float DeltaTime);

	// Getter, Setter 함수들
	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	// ELyraCameraModeBlendFunction enum으로 정의한 블렌딩 함수를 기반으로 BlendAlpha 값을 계산해줌. 
	void SetBlendWeight(float Weight);

	FGameplayTag GetCameraTypeTag() const
	{
		return CameraTypeTag;
	}

	// 카메라 모드의 이름과 BlendWeight를 화면에 출력하는 디버깅 함수.
	virtual void DrawDebug(UCanvas* Canvas) const;

protected:

	// TargetActor가 일반 Actor이면 해당 Actor의 위치를, Pawn인 경우 PawnViewLocation을, 캐릭터인 경우 캡슐 높이까지 고려한 위치를 리턴 해줌. 
	virtual FVector GetPivotLocation() const;
	// TargetActor가 일반 Actor이면 해당 Actor의 회전 값을, Pawn인 경우 ViewRotation을 리턴함. 일반적으로 ViewRotation은 Controller의 ControlRotation.
	virtual FRotator GetPivotRotation() const;

	// PivotLocation, Rotation을 얻어와서 카메라 모드의 결과물인 View의 위치, 회전 값을 업데이트 해줌. ControlRotation, FOV 값도 업데이트 해줌.
	virtual void UpdateView(float DeltaTime);
	// 시간에 따라 계속 해서 증가하는 BlendAlpha 값을 통해 BlendWeight를 0에서 1로 블렌딩 함수에 따라 점점 증가 시켜줌. 
	virtual void UpdateBlending(float DeltaTime);

protected:
	// A tag that can be queried by gameplay code that cares when a kind of camera mode is active
	// without having to ask about a specific mode (e.g., when aiming downsights to get more accuracy)
  // 특정 유형의 카메라 모드가 활성화 되어 있는지 확인할 필요가 있을 때 해당 카메라 모드를 가지고 있는지 직접 질의하지 않고 그냥 태그로 확인할 수 있도록 함.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;

	// 카메라 모드를 통해 산출된 결과물인 시점 정보 구조체. 
	FTICameraModeView View;

	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// How long it takes to blend in this mode.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// Function used for blending.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	ETICameraModeBlendFunction BlendFunction;

	// 커브의 모양을 컨트롤하기 위해 블렌딩 함수에서 사용되는 지수 값. 
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// 계속 해서 증가하는 선형적인 값으로 [0, 1] 범위를 가지며, 이 값을 통해 Blend Weight를 결정함. 
	float BlendAlpha;

	// Blend Alpha와 블렌딩 함수에 의해 계산되는 가중치 값으로, 여러 카메라 모드들이 동시에 적용되고 있을 때 해당 카메라 모드가 최종 시점에 영향을 끼치는 비율.
	float BlendWeight;

protected:
	/** If true, skips all interpolation and puts camera in ideal location.  Automatically set to false next frame. */
	UPROPERTY(transient)
	uint32 bResetInterpolation : 1;
};

/*
* CameraMode의 블렌딩을 처리하는 클래스.
* 내부에 만들어준 CameraMode 배열에 CameraMode들을 넣어 놓고 서로 블렌딩하여 최종 View를 구함.
*/
UCLASS()
class UTICameraModeStack : public UObject
{
	GENERATED_BODY()

public:

	UTICameraModeStack();

	// Stack을 활성화. Stack에 있는 모든 CameraMode에 대해 OnActivation() 함수 호출.
	void ActivateStack();
	// Stack을 비활성화. Stack에 있는 모든 CameraMode에 대해 OnDeactivation() 함수 호출.
	void DeactivateStack();
	// Stack 활성화 여부를 리턴.
	bool IsStackActivate() const { return bIsActive; }
	// Stack에 CameraMode 추가. 기존에 Stack에 있던 CameraMode를 다시 push하는 경우, 해당 CameraMode의 인덱스를 0으로 올려줌. 새 CameraMode를 push할 때도 0번 인덱스에 추가해줌.
	void PushCameraMode(TSubclassOf<UTICameraMode> CameraModeClass);
	// UpdateStack() : UpdateCameraMode 후에 BlendWeight가 1을 넘는 CameraMode를 Stack에서 제거, BlendStack() : Stack의 bottom-top 순서로 BlendWeight에 따라 CameraMode 블렌딩->View 결과 도출.
	bool EvaluateStack(float DeltaTime, FTICameraModeView& OutCameraModeView);
	// Stack에 있는 CameraMode들의 이름, BlendWeight를 출력하는 디버깅 함수.
	void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:

	UTICameraMode* GetCameraModeInstance(TSubclassOf<UTICameraMode> CameraModeClass);
	// UpdateCameraMode를 통해 View를 업데이트, BlendWeight를 갱신.
	void UpdateStack(float DeltaTime);
	// Bottom-Top 순서대로 BlendWeight에 따른 CameraMode View 업데이트.
	void BlendStack(FTICameraModeView& OutCameraModeView) const;

protected:

	bool bIsActive;

	// 카메라 모드 인스턴스들이 저장된 배열.
	UPROPERTY()
	TArray<TObjectPtr<UTICameraMode>> CameraModeInstances;

	// 블렌딩에 사용할 목적으로 카메라 모드이 저장된 배열.
	UPROPERTY()
	TArray<TObjectPtr<UTICameraMode>> CameraModeStack;
};
