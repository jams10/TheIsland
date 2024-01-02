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

	// 액터 함수들 중 PreInitializeComponents() 함수를 포함한 다른 함수들 보다 앞서서 호출되는 초기화 함수.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;

	// Experience 초기화를 위한 준비가 모두 끝난 시점의 다음 프레임에 호출되는 함수.
	void HandleMatchAssignmentIfNotExpectingOne();
};