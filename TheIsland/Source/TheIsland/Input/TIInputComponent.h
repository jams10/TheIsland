#pragma once

#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "InputActionValue.h"
#include "TIInputConfig.h"
#include "TIInputComponent.generated.h"

/*
*	UTIInputComponent
*	InputConfig�� �ִ� InputAction�� �±��� ¦�� �̿�, InputAction�� �Է¿� ���� ����� �Լ��� ���ε� �ϴ� �Լ��� �����ϴ� ������Ʈ.
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

	// ���⼭ �� �� �ֵ��� InputConfig�� Ȱ��ȭ ������ InputAction�� ��� �ִ�.
	// - ���� InputConfig�� ���� InpucAction�� Binding ��Ű��, nullptr�� ��ȯ�Ͽ�, ���ε��ϴµ� �����ϰ� �ȴ�.
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UTIInputComponent::BindAbilityActions(const UTIInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	// AbilityAction�� ���ؼ��� �׳� ��� InputAction�� �� ���ε� ��Ŵ.
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