#pragma once

#include "GameFramework/PlayerState.h"
#include "TIPlayerState.generated.h"

class UTIExperienceDefinition;
class UTIPawnData;

/*
*	ATIPlayerState
*	플레이어 정보를 담고 있는 클래스.
*	플레이어가 조종할 Pawn에 대한 정보를 담고 있는 PawnData도 포함하고 있음.
*/
UCLASS()
class ATIPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATIPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region Actor Interface
	// 액터의 컴포넌트 초기화 완료 후 호출되는 함수.
	virtual void PostInitializeComponents() final;
#pragma endregion

	// Experience가 로드 되었을 때 호출해줄 콜백 함수.	
	void OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience);

	// 현재 사용중인 PawnData.
	UPROPERTY()
	TObjectPtr<const UTIPawnData> PawnData;
};