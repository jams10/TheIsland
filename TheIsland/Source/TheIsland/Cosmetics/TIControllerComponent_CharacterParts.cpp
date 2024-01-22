#include "TIControllerComponent_CharacterParts.h"
#include "TIPawnComponent_CharacterParts.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIControllerComponent_CharacterParts)

UTIControllerComponent_CharacterParts::UTIControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTIControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UTIControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 게임이 끝날 때 EndPlay()에서는 추가했던 모든 캐릭터 파츠들을 제거.
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
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

void UTIControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대해서는 Character Parts를 전부 제거.
	if (UTIPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UTIPawnComponent_CharacterParts>() : nullptr)
	{
		for (FTIControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 가지고 있는 Character Parts를 추가해줌.
	if (UTIPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UTIPawnComponent_CharacterParts>() : nullptr)
	{
		for (FTIControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}

void UTIControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UTIPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FTIControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset(); // reset은 인덱스만 0으로 바꿔줌. resolve한 메모리 공간은 날려주지 않음.
}

void UTIControllerComponent_CharacterParts::AddCharacterPart(const FTICharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}
