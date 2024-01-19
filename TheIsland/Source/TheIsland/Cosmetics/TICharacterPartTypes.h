#pragma once

#include "CoreMinimal.h"
#include "TICharacterPartTypes.generated.h"

/*
*	FTICharacterPartHandle
*	Character Part Entry를 추가 함으로써 만들어지는 핸들. 나중에 해당 Character Part를 제거할 때 사용함.
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
*	캐릭터 파츠 내용을 담은 구조체. 파츠를 나타내는 액터 클래스와 어디에 붙여줄 지에 대한 소켓 정보를 저장.
*/
USTRUCT(BlueprintType)
struct FTICharacterPart
{
	GENERATED_BODY()

	// 파츠로 생성한 Actor의 Class 정보를 가지고 있음 -> Actor BP를 하나의 파츠로 생각하면 됨.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	// Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보 (따로 정의되어 있지 않으면 그냥 Root에 붙임)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};