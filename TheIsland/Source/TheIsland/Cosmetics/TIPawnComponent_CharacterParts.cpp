#include "TIPawnComponent_CharacterParts.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIPawnComponent_CharacterParts)

FGameplayTagContainer FTICharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	// Entries를 순회하며,
	for (const FTIAppliedCharacterPartEntry& Entry : Entries)
	{
		// Part Actor가 이전에 생성되어 SpawnedComponent에 캐싱되어 있다면,
		if (Entry.SpawnedComponent)
		{
			// 해당 Actor를 IGameplayTagAssetInterface를 통해 GameplayTag를 검색한다:
			// - 현재 우리의 TaggedActor는 IGameplayTagAssetInterface를 상속 받지 않았으므로 그냥 넘어갈 것이다.
			// - 후일 우리가 각 Part에 대해 GameplayTag를 넣고 싶다면 이걸 상속 받아 정의해 주어야 함.
			//	   - 예를 들어, 특정 Lv100 이상 장착 가능한 장비를 만들고 싶다면 태그 인터페이스를 구현해 주어야 함.
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
	// 전달된 AppliedCharacterPartEntry의 Part Class가 제대로 세팅되어 있다면
	if (Entry.Part.PartClass != nullptr)
	{
		// OwnerComponent의 Owner가 속한 World를 반환.
		UWorld* World = OwnerComponent->GetWorld();

		// 캐릭터 파츠를 어느 씬 컴포넌트에 붙일지 결정함. ( Mesh? SceneRoot? )
		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			// 붙일 Component에 대해 캐릭터 파츠 정보에 담긴 SocketName을 통해 SocketTransform을 얻어옴.
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			// 우리는 Actor-Actor의 결합이므로, ChildActorComponent를 활용해 붙여줌.
			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);
			// 참고로 RegisterComponent를 통해 마지막으로 RenderWorld만 FScene에 변경 내용을 전달(혹은 생성).
			// 컴포넌트를 붙였으니 렌더링 관련 데이터를 다 처리해서 렌더링을 시작해 달라고 요청.
			PartComponent->RegisterComponent();

			// ChildActorComponent에서 생성한 Actor를 반환하여
			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				// 해당 Actor가 Parent인 TIComponent_CharacterPart의 Owner Actor보다 먼저 Tick이 실행되지 않도록 선행 조건을 붙임.
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
		// DestroyComponent() 함수를 호출했다고 해서 바로 제거되지는 않고 GC에 마킹만 해놓게 됨.
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FTICharacterPartHandle FTICharacterPartList::AddEntry(FTICharacterPart NewPart)
{
	// PawnComponent의 CharacterPartList가 PartHandle을 관리하고, 이를 ControllerComponent_CharacterParts에 전달함.
	FTICharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	// Authority가 있다면, AppliedCharacterPartEntry를 Entries에 추가함.
	// OwnerComponent->GetOwner() == B_Hero_ShooterMannequin
	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FTIAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;

		// 여기서 실제 Actor를 생성하고, OwnerComponent의 Owner Actor에 Actor끼리 RootComponent로 Attach 시킴.
		if (SpawnActorForEntry(NewEntry))
		{
			// BroadcastChanged를 통해, OwnerComponent에서 Owner의 SkeletalMeshComponent를 활용하여, Animation 및 Physics를 Re-Initialize 해준다.
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

		// 제거할 경우, PartHandle을 활용함.
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
	// Character를 활용하여, 최상위 SkeletalMesh를 반환함.
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
	// Parent에 SkeletalMeshComponent가 있으면 반환하고
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}
	// 그리고 RootComponent도 확인하고
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}
	// 그냥 nullptr 반환.
	return nullptr;
}

FGameplayTagContainer UTIPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	// 현재 장착된 CharacterPartList의 Merge된 Tags를 반환함.
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		// 만약 GameplayTag를 통해 필터링할 경우, 필터링해서 진행함.
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		// 필터링할 GameplayTag가 없으면 그냥 반환
		return Result;
	}
}

void UTIPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// 현재 Owner의 SkeletalMeshComponent를 반환함.
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// BodyMeshes를 통해, GameplayTag를 활용하여, 알맞은 SkeletalMesh로 재설정해줌.
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshses.SelectBestBodyStyle(MergedTags);

		// SkeletalMesh를 초기화 및 Animation 초기화 시켜줌.
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		// PhysicsAsset을 초기화 해줌.
		if (UPhysicsAsset* PhysicsAsset = BodyMeshses.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}
