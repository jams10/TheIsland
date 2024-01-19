#pragma once

#include "CoreMinimal.h"
#include "TICharacterPartTypes.generated.h"

/*
*	FTICharacterPartHandle
*	Character Part Entry�� �߰� �����ν� ��������� �ڵ�. ���߿� �ش� Character Part�� ������ �� �����.
*/
USTRUCT(BlueprintType)
struct FTICharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

/*
*	FTICharacterPart
*	ĳ���� ���� ������ ���� ����ü. ������ ��Ÿ���� ���� Ŭ������ ��� �ٿ��� ���� ���� ���� ������ ����.
*/
USTRUCT(BlueprintType)
struct FTICharacterPart
{
	GENERATED_BODY()

	// ������ ������ Actor�� Class ������ ������ ���� -> Actor BP�� �ϳ��� ������ �����ϸ� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	// Actor�� ��ȯ�Ͽ� ���̴µ� Ư�� Bone�� ���̱� ���� ���� (���� ���ǵǾ� ���� ������ �׳� Root�� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};