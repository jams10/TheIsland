#include "TIControllerComponent_CharacterParts.h"
#include "TIPawnComponent_CharacterParts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIControllerComponent_CharacterParts)

UTIControllerComponent_CharacterParts::UTIControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

UTIPawnComponent_CharacterParts* UTIControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 생각해보면, 우리는 앞서 PawnComponent_CharacterParts를 상속 받는 B_MannequinPawnCosmetics를 이미 B_Hero_ShooterMannequin에 추가했음.
		// B_MannequinPawnCosmetics가 반환되길 기대함.
		return ControlledPawn->FindComponentByClass<UTIPawnComponent_CharacterParts>();
	}
	return nullptr;
}

void UTIControllerComponent_CharacterParts::AddCharacterPartInternal(const FTICharacterPart& NewPart)
{
	// CharacterPart 데이터를 관리하기 위해 만들어준 CharacterParts 배열에 새로운 Character Part를 추가해줌.
	FTIControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	// PawnComponent_CharacterParts 컴포넌트 클래스에 새로 추가한 CharacterPart를 넘겨 이를 활용, 실제 스폰을 할 수 있도록 함.
	if (UTIPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}

}

void UTIControllerComponent_CharacterParts::AddCharacterPart(const FTICharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}
