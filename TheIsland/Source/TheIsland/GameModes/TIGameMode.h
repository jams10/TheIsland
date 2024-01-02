#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TIGameMode.generated.h"

/**
 *	ATIGameMode
 *	게임 모드 클래스. 기존 게임 모드의 역할은 Experience가 대신하고, 게임 모드는 Experience를 관리하는 클래스들의 초기화 작업을 수행함.
 */
UCLASS()
class THEISLAND_API ATIGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATIGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};