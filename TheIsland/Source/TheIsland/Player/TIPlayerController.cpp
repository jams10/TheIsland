#include "TIPlayerController.h"
#include "TheIsland/Camera/TIPlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPlayerController)

ATIPlayerController::ATIPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// PlayerCameraManager�� �츮�� ���� Ŭ������ ����.
	PlayerCameraManagerClass = ATIPlayerCameraManager::StaticClass();
}
