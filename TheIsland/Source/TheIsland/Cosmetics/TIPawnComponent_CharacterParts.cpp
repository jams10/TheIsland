#include "TIPawnComponent_CharacterParts.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPawnComponent_CharacterParts)

FGameplayTagContainer FTICharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	// Entries�� ��ȸ�ϸ�,
	for (const FTIAppliedCharacterPartEntry& Entry : Entries)
	{
		// Part Actor�� ������ �����Ǿ� SpawnedComponent�� ĳ�̵Ǿ� �ִٸ�,
		if (Entry.SpawnedComponent)
		{
			// �ش� Actor�� IGameplayTagAssetInterface�� ���� GameplayTag�� �˻��Ѵ�:
			// - ���� �츮�� TaggedActor�� IGameplayTagAssetInterface�� ��� ���� �ʾ����Ƿ� �׳� �Ѿ ���̴�.
			// - ���� �츮�� �� Part�� ���� GameplayTag�� �ְ� �ʹٸ� �̰� ��� �޾� ������ �־�� ��.
			//	   - ���� ���, Ư�� Lv100 �̻� ���� ������ ��� ����� �ʹٸ� �±� �������̽��� ������ �־�� ��.
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(/*inout*/ Result);
			}
		}
	}

	return Result;
}

bool FTICharacterPartList::SpawnActorForEntry(FTIAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;
	// ���޵� AppliedCharacterPartEntry�� Part Class�� ����� ���õǾ� �ִٸ�
	if (Entry.Part.PartClass != nullptr)
	{
		// OwnerComponent�� Owner�� ���� World�� ��ȯ.
		UWorld* World = OwnerComponent->GetWorld();

		// ĳ���� ������ ��� �� ������Ʈ�� ������ ������. ( Mesh? SceneRoot? )
		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			// ���� Component�� ���� ĳ���� ���� ������ ��� SocketName�� ���� SocketTransform�� ����.
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			// �츮�� Actor-Actor�� �����̹Ƿ�, ChildActorComponent�� Ȱ���� �ٿ���.
			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);
			// ����� RegisterComponent�� ���� ���������� RenderWorld�� FScene�� ���� ������ ����(Ȥ�� ����).
			// ������Ʈ�� �ٿ����� ������ ���� �����͸� �� ó���ؼ� �������� ������ �޶�� ��û.
			PartComponent->RegisterComponent();

			// ChildActorComponent���� ������ Actor�� ��ȯ�Ͽ�
			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				// �ش� Actor�� Parent�� TIComponent_CharacterPart�� Owner Actor���� ���� Tick�� ������� �ʵ��� ���� ������ ����.
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}
			Entry.SpawnedComponent = PartComponent;
			bCreatedAnyActor = true;
		}
	}

	return bCreatedAnyActor;
}

void FTICharacterPartList::DestroyActorForEntry(FTIAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent)
	{
		// DestroyComponent() �Լ��� ȣ���ߴٰ� �ؼ� �ٷ� ���ŵ����� �ʰ� GC�� ��ŷ�� �س��� ��.
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FTICharacterPartHandle FTICharacterPartList::AddEntry(FTICharacterPart NewPart)
{
	// PawnComponent�� CharacterPartList�� PartHandle�� �����ϰ�, �̸� ControllerComponent_CharacterParts�� ������.
	FTICharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	// Authority�� �ִٸ�, AppliedCharacterPartEntry�� Entries�� �߰���.
	// OwnerComponent->GetOwner() == B_Hero_ShooterMannequin
	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FTIAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;

		// ���⼭ ���� Actor�� �����ϰ�, OwnerComponent�� Owner Actor�� Actor���� RootComponent�� Attach ��Ŵ.
		if (SpawnActorForEntry(NewEntry))
		{
			// BroadcastChanged�� ����, OwnerComponent���� Owner�� SkeletalMeshComponent�� Ȱ���Ͽ�, Animation �� Physics�� Re-Initialize ���ش�.
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

void FTICharacterPartList::RemoveEntry(FTICharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FTIAppliedCharacterPartEntry& Entry = *EntryIt;

		// ������ ���, PartHandle�� Ȱ����.
		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
		}
	}
}

UTIPawnComponent_CharacterParts::UTIPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	CharacterPartList(this)
{
}

FTICharacterPartHandle UTIPawnComponent_CharacterParts::AddCharacterPart(const FTICharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void UTIPawnComponent_CharacterParts::RemoveCharacterPart(FTICharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

USkeletalMeshComponent* UTIPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	// Character�� Ȱ���Ͽ�, �ֻ��� SkeletalMesh�� ��ȯ��.
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}
	return nullptr;
}

USceneComponent* UTIPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	// Parent�� SkeletalMeshComponent�� ������ ��ȯ�ϰ�
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}
	// �׸��� RootComponent�� Ȯ���ϰ�
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}
	// �׳� nullptr ��ȯ.
	return nullptr;
}

FGameplayTagContainer UTIPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	// ���� ������ CharacterPartList�� Merge�� Tags�� ��ȯ��.
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		// ���� GameplayTag�� ���� ���͸��� ���, ���͸��ؼ� ������.
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		// ���͸��� GameplayTag�� ������ �׳� ��ȯ
		return Result;
	}
}

void UTIPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// ���� Owner�� SkeletalMeshComponent�� ��ȯ��.
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// BodyMeshes�� ����, GameplayTag�� Ȱ���Ͽ�, �˸��� SkeletalMesh�� �缳������.
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshses.SelectBestBodyStyle(MergedTags);

		// SkeletalMesh�� �ʱ�ȭ �� Animation �ʱ�ȭ ������.
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		// PhysicsAsset�� �ʱ�ȭ ����.
		if (UPhysicsAsset* PhysicsAsset = BodyMeshses.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}
