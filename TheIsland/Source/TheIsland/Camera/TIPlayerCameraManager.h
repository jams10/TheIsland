#pragma once

#include "Camera/PlayerCameraManager.h"
#include "TIPlayerCameraManager.generated.h"

#define TI_CAMERA_DEFAULT_FOV 80.f
#define TI_CAMREA_DEFAULT_PITCH_MIN -89.0f
#define TI_CAMREA_DEFAULT_PITCH_MAX 89.0f

/*
*   ATIPlayerCameraManager
*	PlayerCameraManager 클래스는 카메라 관리자 클래스. 여기서는 단순히 기본 FOV와 Pitch 값 범위를 설정해주고 있음.
*/
UCLASS()
class ATIPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ATIPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};