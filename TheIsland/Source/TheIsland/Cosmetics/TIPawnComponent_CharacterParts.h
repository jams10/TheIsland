#pragma once

#include "Components/PawnComponent.h"
#include "TICharacterPartTypes.h"
#include "TICosmeticAnimationTypes.h"
#include "TIPawnComponent_CharacterParts.generated.h"

/*
*	FTIAppliedCharacterPartEntry
*	���� ĳ���Ϳ� ����Ǵ� ĳ���� ���� ����.
*/
USTRUCT()
struct FTIAppliedCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part�� ����(��Ÿ ������)
	UPROPERTY()
	FTICharacterPart Part;

	// FTICharacterPartList���� �Ҵ� ���� Part �ڵ� ��(FTIControllerCharacterPartEntry�� Handle ���� ���ƾ� �� -> Handle ���� ������ ������ Part)
	// �������� �������� ĳ���� ������ ���� �ڵ��� �ε���. (���� ����)
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// �ν��Ͻ�ȭ �� Character Part �� Actor.
	// ���� �� ���� ������ �� ChildActorComponent�� �����. UChildActorComponent�� ���ο� spawn�� actor Ŭ������ �ν��Ͻ�ȭ �� actor�� TObjectPtr�� ��� ����.
	// ChildActorComponent�� attach child�� root ������Ʈ�� �ٰ� ��.
	// ������ �����Ǵ� ���� �ν��Ͻ�. (Ŭ���̾�Ʈ ����)
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/*
*	FTICharacterPartList
*	PawnComponent_CharacterParts���� ������ Character Parts�� �����ϴ� ����ü. 
*	������ ĳ���Ϳ� ����� ĳ���� ������ ����Ʈ�̸�, Ŭ���̾�Ʈ�� ������.
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

	// ĳ���� ���� �ν��Ͻ��� ������ ����, ĳ���Ϳ� �ٿ��ִ� �Լ�.
	bool SpawnActorForEntry(FTIAppliedCharacterPartEntry& Entry);
	// ĳ���� ���� �ν��Ͻ��� �����ϴ� �Լ�.
	void DestroyActorForEntry(FTIAppliedCharacterPartEntry& Entry);

	// ������ �ν��Ͻ�ȭ �� ĳ���� �������� �����ϴ� ĳ���� ���� ����Ʈ�� ĳ���� ������ �߰��ϴ� �Լ�.
	FTICharacterPartHandle AddEntry(FTICharacterPart NewPart);
	// ������ �ν��Ͻ�ȭ �� ĳ���� �������� �����ϴ� ĳ���� ���� ����Ʈ���� ĳ���� ������ �����ϴ� �Լ�.
	void RemoveEntry(FTICharacterPartHandle Handle);

	// ĳ���Ϳ� �������� ĳ���� ���� ����Ʈ.
	UPROPERTY()
	TArray<FTIAppliedCharacterPartEntry> Entries;

	// �ش� TICharacterPartList�� Owner�� PawnComponent.
	UPROPERTY()
	TObjectPtr<UTIPawnComponent_CharacterParts> OwnerComponent;

	// �ռ� ���Ҵ� PartHandle�� ���� �Ҵ� �� �����ϴ� ����.
	int32 PartHandleCounter = 0;
};

/*
*	UTIPawnComponent_CharacterParts
*	��� Ŭ���̾�Ʈ���� owner pawn�� �����Ǵ� ������ cosmetic ���͵��� �����ϴ� ������Ʈ.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class UTIPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:

	UTIPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// �� customization ������Ʈ�� �پ� �ִ� ���Ϳ� ĳ���� ������ �߰��ϴ� �Լ�. �ݵ�� authority���� ȣ��Ǿ�� ��.
	FTICharacterPartHandle AddCharacterPart(const FTICharacterPart& NewPart);

	// ������ �߰��� ĳ���� ������ �����ϴ� �Լ�. �ݵ�� authority���� ȣ��Ǿ�� ��.
	void RemoveCharacterPart(FTICharacterPartHandle Handle);

	// �θ� ���Ͱ� ACharacter�� ��� �޾� ������� ���, Mesh ������Ʈ�� �����ϰ�, �׷��� ������ nullptr�� ������.
	USkeletalMeshComponent* GetParentMeshComponent() const;
	// ĳ���� ������ �ٿ��� ������Ʈ(�޽� ������Ʈ, �� ��Ʈ ������Ʈ ��)�� �����ϴ� �Լ�.
	USceneComponent* GetSceneComponentToAttachTo() const;
	// ���� �پ� �ִ� ĳ���� �������� ���յ� ���� �÷��� �±� ������ �����ϴ� �Լ�. 
	// ���������� ������ ��Ʈ�� �����ϴ� �±׵鸸 ���͸� ��.
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	// �� ĳ���� ������ �߰� ���� �� GameplayTag�� ���� SkeletalMesh�� �����ϰų�, �ʱ�ȭ �� ���� Animation, Physics �ּ��� �ʱ�ȭ ���ִ� �Լ�.
	void BroadcastChanged();

protected:

	// ĳ���� ���� ����Ʈ.
	UPROPERTY()
	FTICharacterPartList CharacterPartList;

	// �ִϸ��̼� ������ ���� �޽ÿ� ���� ����.
	// character part cosmetic tag�鿡 ����Ͽ� �ִϸ��̼��� ����ϱ� ���� body style mesh�� �����ϴ� ��Ģ.
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FTIAnimBodyStyleSelectionSet BodyMeshses;
};