#include "TIPlayerController.h"
#include "TheIsland/Camera/TIPlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPlayerController)

ATIPlayerController::ATIPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// PlayerCameraManager를 우리가 만든 클래스로 설정.
	PlayerCameraManagerClass = ATIPlayerCameraManager::StaticClass();
}
