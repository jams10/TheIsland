// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheIsland.h"
#include "Modules/ModuleManager.h"

/**
 * FTIGameModule
 * 기본 모듈 메인 클래스인 FDefaultGameModuleImpl 클래스를 상속 받은 클래스. 프로젝트 이름에 맞춰 'TI'를 넣어 주었음.
 * 모듈의 메인 클래스의 경우 어디에서나 사용할 수 있기 때문에 사실상 모듈이 엔진에 로드 되어 있는 동안에 사용할 수 있는 싱글톤과 비슷함.
 */
class FTIGameModule : public FDefaultGameModuleImpl
{
	// 모듈의 dll이 엔진에 로드 되고, 모듈 객체가 생성된 후에 호출됨.
	// 에디터 초기화 단계(Initializing)에서 호출됨. 모듈의 초기 설정 및 필요한 동작을 수행함.
	virtual void StartupModule() override;

	// 모듈의 dll이 엔진에서 내려지고(unload), 모듈 객체가 제거되기 전에 호출됨.
	// 엔진의 실행이 완전히 종료된 후에 호출됨.
	virtual void ShutdownModule() override;
};

void FTIGameModule::StartupModule()
{

}

void FTIGameModule::ShutdownModule()
{

}

// FTIGameModule을 게임에서 사용할 주 게임 모듈로 설정함.
IMPLEMENT_PRIMARY_GAME_MODULE(FTIGameModule, TheIsland, "TheIsland" );
