#pragma once

#include "Engine/GameInstance.h"
#include "TIGameInstance.generated.h"

/*
* GameInstance는 게임 프로세스(.exe)에서 하나만 존재하는 객체. (== high-level manager object)
* - 게임이 켜질 때, 만들어지고, 게임이 꺼지기 전까지 살아 있다.
* - Editor 상에서는 PIE로 실행 될때마다 하나씩 생성됨. 즉, 에디터에서는 복수개의 GameInstance가 존재할 수 있음!
*/
UCLASS()
class UTIGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTIGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	* UGameInstance's interfaces
	*/
	virtual void Init() override;
	virtual void Shutdown() override;
};