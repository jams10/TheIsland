#include "TIExperienceManagerComponent.h"
#include "TIExperienceDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIExperienceManagerComponent)

UTIExperienceManagerComponent::UTIExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UTIExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		// Experience가 이미 로드 되어 있다면 현재 로드된 Experience를 전달하면서 인자로 받아준 Delegate를 그대로 실행.
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		// OnExperienceLoaded multicast 델리게이트의 호출 리스트에 Delegate 인스턴스를 추가하여, Experience가 로드 되었을 때 응답 받을 수 있도록 함.
		OnExperienceLoaded.Add(MoveTemp(Delegate)); // Delegate 인스턴스 복사 비용을 낮추기 위해 Move를 통해 인스턴스 전달.
	}
}
