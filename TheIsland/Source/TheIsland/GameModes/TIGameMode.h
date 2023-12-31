﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TIGameMode.generated.h"

class UTIExperienceDefinition;
class UTIPawnData;

/**
 *	ATIGameMode
 *	게임 모드 클래스. 기존 게임 모드의 역할은 Experience가 대신하고, 게임 모드는 Experience를 관리하는 클래스들의 초기화 작업을 수행함.
 */
UCLASS()
class THEISLAND_API ATIGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATIGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 액터 함수들 중 PreInitializeComponents() 함수를 포함한 다른 함수들 보다 앞서서 호출되는 초기화 함수.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) final;
	// GameState가 스폰된 뒤에 PreInitializeComponents() 함수에서 호출하는 함수. GameState 액터를 기본 세팅 값으로 초기화 함.
	virtual void InitGameState() final;

	// Transform 정보를 통해 실제로 Pawn을 spawn하는 함수.
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	// GameMode에 설정해준 DefaultPawnClass를 리턴하는 함수. 우리는 이를 override 하여 Experience의 PawnData에 있는 Pawn class를 리턴하도록 함.
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;

	// 조종할 Pawn 생성의 시작점이 되는 함수. Experience가 로드 되었을 때 Pawn을 spawn할 수 있도록 override 해줌.
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	// Controller가 보유한 PlayerState로부터 PawnData를 얻어와 리턴하는 함수.
	const UTIPawnData* GetPawnDataForController(const AController* InController) const;

#pragma region Experience
	// Experience 로드를 위한 준비가 모두 끝난 시점의 다음 프레임에 호출되는 함수.
	void HandleMatchAssignmentIfNotExpectingOne();
	// 여러 조건에 맞는 Experience가 선택 되고, 해당 Experience의 asset id를 받아서 Experience 로드 작업을 시작하는 함수.
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	// Experience가 로드 되었을 때 호출해줄 콜백 함수. Experience가 로드 된 이후의 작업을 처리.
	void OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience);
	// Experience가 로드 되었는지 여부를 리턴하는 함수.
	bool IsExperienceLoaded() const;
#pragma endregion
};