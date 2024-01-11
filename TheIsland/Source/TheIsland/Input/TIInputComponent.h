#pragma once

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "TIInputConfig.h"
#include "TIInputComponent.generated.h"

/*
*	UTIInputComponent
*	InputConfig에 있는 InputAction과 태그의 짝을 이용, InputAction과 입력에 따라 실행될 함수를 바인딩 하는 함수를 제공하는 컴포넌트.
*/
UCLASS()
class UTIInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	UTIInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* member methods */
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UTIInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UTIInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
void UTIInputComponent::BindNativeAction(const UTIInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	// 여기서 알 수 있듯이 InputConfig는 활성화 가능한 InputAction을 담고 있다.
	// - 만약 InputConfig에 없는 InpucAction을 Binding 시키면, nullptr를 반환하여, 바인딩하는데 실패하게 된다.
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UTIInputComponent::BindAbilityActions(const UTIInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	// AbilityAction에 대해서는 그냥 모든 InputAction에 다 바인딩 시킴.
	for (const FTIInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}