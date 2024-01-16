#include "GameFeatureAction_WorldActionBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_WorldActionBase)

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// 월드를 순회하면서
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// 앞서 ExperienceManagerComponent에서 GameFeatureAction을 활성화하면서, Context에 World를 넣어 주었음.
		// 따라서, 모든 월드 Context들을 순회하면서, 인자로 넘어온 World Context와 동일한 경우에만 AddToWorld() 함수를 호출함.
		// 일반적으로 인게임에서는 월드가 하나밖에 없어서 문제가 되지 않지만, 에디터 상에서는 에디터 상에서의 월드와 Play in editor 상의 월드가 따로 존재하기 때문에 이를 필터링 해주는 것임.
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			// WorldActionBase의 Interface인 AddToWorld 호출.
			AddToWorld(WorldContext, Context);
		}
	}
}