#include "TIPlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPlayerCameraManager)

ATIPlayerCameraManager::ATIPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultFOV = TI_CAMERA_DEFAULT_FOV;
	ViewPitchMin = TI_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = TI_CAMERA_DEFAULT_PITCH_MAX;
}