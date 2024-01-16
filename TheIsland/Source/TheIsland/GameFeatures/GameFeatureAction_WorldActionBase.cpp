#include "GameFeatureAction_WorldActionBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_WorldActionBase)

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// ���带 ��ȸ�ϸ鼭
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// �ռ� ExperienceManagerComponent���� GameFeatureAction�� Ȱ��ȭ�ϸ鼭, Context�� World�� �־� �־���.
		// ����, ��� ���� Context���� ��ȸ�ϸ鼭, ���ڷ� �Ѿ�� World Context�� ������ ��쿡�� AddToWorld() �Լ��� ȣ����.
		// �Ϲ������� �ΰ��ӿ����� ���尡 �ϳ��ۿ� ��� ������ ���� ������, ������ �󿡼��� ������ �󿡼��� ����� Play in editor ���� ���尡 ���� �����ϱ� ������ �̸� ���͸� ���ִ� ����.
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			// WorldActionBase�� Interface�� AddToWorld ȣ��.
			AddToWorld(WorldContext, Context);
		}
	}
}