#pragma once

#include "Components/PawnComponent.h"
#include "TICharacterPartTypes.h"
#include "TICosmeticAnimationTypes.h"
#include "TIPawnComponent_CharacterParts.generated.h"

/*
*	FTIAppliedCharacterPartEntry
*	실제 캐릭터에 적용되는 캐릭터 파츠 단위.
*/
USTRUCT()
struct FTIAppliedCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part의 정의(메타 데이터)
	UPROPERTY()
	FTICharacterPart Part;

	// FTICharacterPartList에서 할당 받은 Part 핸들 값(FTIControllerCharacterPartEntry의 Handle 값과 같아야 함 -> Handle 값이 같으면 동일한 Part)
	// 유저에게 리턴해줄 캐릭터 파츠에 대한 핸들의 인덱스. (서버 전용)
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// 인스턴스화 된 Character Part 용 Actor.
	// 액터 간 연결 시켜줄 때 ChildActorComponent를 사용함. UChildActorComponent는 내부에 spawn할 actor 클래스와 인스턴스화 된 actor를 TObjectPtr로 들고 있음.
	// ChildActorComponent는 attach child로 root 컴포넌트에 붙게 됨.
	// 실제로 스폰되는 액터 인스턴스. (클라이언트 전용)
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/*
*	FTICharacterPartList
*	PawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 구조체. 
*	실제로 캐릭터에 적용될 캐릭터 파츠의 리스트이며, 클라이언트로 복제됨.
*/
USTRUCT(BlueprintType)
struct FTICharacterPartList
{
	GENERATED_BODY()

	FTICharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FTICharacterPartList(UTIPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	FGameplayTagContainer CollectCombinedTags() const;

	// 캐릭터 파츠 인스턴스를 실제로 스폰, 캐릭터에 붙여주는 함수.
	bool SpawnActorForEntry(FTIAppliedCharacterPartEntry& Entry);
	// 캐릭터 파츠 인스턴스를 제거하는 함수.
	void DestroyActorForEntry(FTIAppliedCharacterPartEntry& Entry);

	// 실제로 인스턴스화 될 캐릭터 파츠들을 관리하는 캐릭터 파츠 리스트에 캐릭터 파츠를 추가하는 함수.
	FTICharacterPartHandle AddEntry(FTICharacterPart NewPart);
	// 실제로 인스턴스화 될 캐릭터 파츠들을 관리하는 캐릭터 파츠 리스트에서 캐릭터 파츠를 제거하는 함수.
	void RemoveEntry(FTICharacterPartHandle Handle);

	// 캐릭터에 적용해줄 캐릭터 파츠 리스트.
	UPROPERTY()
	TArray<FTIAppliedCharacterPartEntry> Entries;

	// 해당 TICharacterPartList의 Owner인 PawnComponent.
	UPROPERTY()
	TObjectPtr<UTIPawnComponent_CharacterParts> OwnerComponent;

	// 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수.
	int32 PartHandleCounter = 0;
};

/*
*	UTIPawnComponent_CharacterParts
*	모든 클라이언트에서 owner pawn에 부착되는 스폰된 cosmetic 액터들을 관리하는 컴포넌트.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class UTIPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:

	UTIPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 이 customization 컴포넌트가 붙어 있는 액터에 캐릭터 파츠를 추가하는 함수. 반드시 authority에서 호출되어야 함.
	FTICharacterPartHandle AddCharacterPart(const FTICharacterPart& NewPart);

	// 이전에 추가된 캐릭터 파츠를 제거하는 함수. 반드시 authority에서 호출되어야 함.
	void RemoveCharacterPart(FTICharacterPartHandle Handle);

	// 부모 액터가 ACharacter를 상속 받아 만들어진 경우, Mesh 컴포넌트를 리턴하고, 그렇지 않으면 nullptr를 리턴함.
	USkeletalMeshComponent* GetParentMeshComponent() const;
	// 캐릭터 파츠를 붙여줄 컴포넌트(메쉬 컴포넌트, 씬 루트 컴포넌트 등)를 리턴하는 함수.
	USceneComponent* GetSceneComponentToAttachTo() const;
	// 현재 붙어 있는 캐릭터 파츠들의 결합된 게임 플레이 태그 집합을 리턴하는 함수. 
	// 선택적으로 지정된 루트로 시작하는 태그들만 필터링 함.
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	// 새 캐릭터 파츠를 추가 했을 때 GameplayTag에 따라 SkeletalMesh를 변경하거나, 초기화 및 관련 Animation, Physics 애셋을 초기화 해주는 함수.
	void BroadcastChanged();

protected:

	// 캐릭터 파츠 리스트.
	UPROPERTY()
	FTICharacterPartList CharacterPartList;

	// 애니메이션 적용을 위한 메시와 연결 고리.
	// character part cosmetic tag들에 기반하여 애니메이션을 재생하기 위해 body style mesh를 선택하는 규칙.
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FTIAnimBodyStyleSelectionSet BodyMeshses;
};