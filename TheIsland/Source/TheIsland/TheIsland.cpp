// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheIsland.h"
#include "Modules/ModuleManager.h"

/**
 * FTIGameModule
 * �⺻ ��� ���� Ŭ������ FDefaultGameModuleImpl Ŭ������ ��� ���� Ŭ����. ������Ʈ �̸��� ���� 'TI'�� �־� �־���.
 * ����� ���� Ŭ������ ��� ��𿡼��� ����� �� �ֱ� ������ ��ǻ� ����� ������ �ε� �Ǿ� �ִ� ���ȿ� ����� �� �ִ� �̱���� �����.
 */
class FTIGameModule : public FDefaultGameModuleImpl
{
	// ����� dll�� ������ �ε� �ǰ�, ��� ��ü�� ������ �Ŀ� ȣ���.
	// ������ �ʱ�ȭ �ܰ�(Initializing)���� ȣ���. ����� �ʱ� ���� �� �ʿ��� ������ ������.
	virtual void StartupModule() override;

	// ����� dll�� �������� ��������(unload), ��� ��ü�� ���ŵǱ� ���� ȣ���.
	// ������ ������ ������ ����� �Ŀ� ȣ���.
	virtual void ShutdownModule() override;
};

void FTIGameModule::StartupModule()
{

}

void FTIGameModule::ShutdownModule()
{

}

// FTIGameModule�� ���ӿ��� ����� �� ���� ���� ������.
IMPLEMENT_PRIMARY_GAME_MODULE(FTIGameModule, TheIsland, "TheIsland" );
