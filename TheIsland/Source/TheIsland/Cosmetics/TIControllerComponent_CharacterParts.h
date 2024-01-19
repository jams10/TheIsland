#pragma once

#include "Components/ControllerComponent.h"
#include "TICharacterPartTypes.h"
#include "TIControllerComponent_CharacterParts.generated.h"

/*
*	FTIControllerCharacterPartEntry
*	ControllerComponent가 소유하는 Character Parts.
*/
USTRUCT()
struct FTIControllerCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part에 대한 정의(메타 데이터 == Meta Data)
	UPROPERTY(EditAnywhere)
	FTICharacterPart Part;

	// Character Part 핸들(고유값) - Controller가 Possess 하고 있는 Pawn에서 생성한(인스턴스) Character Part에 대해 고유한 값을 부여한 핸들 값.
	// 유저가 게임을 나갔을 때 부착한 액터들을 해제 해주어야 하는데 이 때 액터 핸들 값을 비교하는데 사용할 수 있음.
	FTICharacterPartHandle Handle;
};

/*
*	UTIControllerComponent_CharacterParts
*	Controller가 Pawn을 Possess 했을 때, Pawn에 어떤 Cosmetic Actor들을 생성해 줄 지 결정하는 컴포넌트.
*	캐릭터 파츠 데이터를 유저 관점에서 관리하는 Component.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class UTIControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UTIControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FTIControllerCharacterPartEntry> CharacterParts;
};