#include "TICosmeticAnimationTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TICosmeticAnimationTypes)

USkeletalMesh* FTIAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	// MeshRule�� ��ȸ�ϸ�, CosmeticTags �䱸 ���ǿ� �´� MeshRule�� ã�� SkeletalMesh�� ��ȯ��.
	for (const FTIAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	return DefaultMesh; // �ƹ��͵� �����Ǿ� ���� ���� ���̷�Ż �޽� ����.
}