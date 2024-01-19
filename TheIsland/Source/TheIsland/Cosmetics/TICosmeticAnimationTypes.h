#pragma once

#include "GameplayTagContainer.h"
#include "TICosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FTIAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	// AnimLayer를 적용할 대상 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// Cosmetic Tag라고 생각하면 됨.
	// 예를 들어 장비의 레벨 제한, 스탯 제한을 tag로 정의 할 수 있음.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FTIAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	// AnimLayer를 적용할 SkeletalMesh를 들고 있음 -> Animation - Mesh간 Rules를 MeshRules라고 생각하면 됨.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTIAnimBodyStyleSelectionEntry> MeshRules;

	// 그냥 디폴트로 적용할 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// Physics Asset은 하나로 통일함 -> 즉 모든 Animation의 Physics 속성은 공유함.
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};