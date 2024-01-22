#include "TICosmeticAnimationTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TICosmeticAnimationTypes)

USkeletalMesh* FTIAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	// MeshRule을 순회하며, CosmeticTags 요구 조건에 맞는 MeshRule을 찾아 SkeletalMesh를 반환함.
	for (const FTIAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	return DefaultMesh; // 아무것도 장착되어 있지 않은 스켈레탈 메쉬 리턴.
}