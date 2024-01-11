// Copyright jams10. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "TIInputConfig.generated.h"

class UInputAction;

/*
*	FTIInputAction
*	InputAction을 GameplayTag와 매핑 시켜주는 래퍼 구조체.
*/
USTRUCT(BlueprintType)
struct FTIInputAction
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 *	UTIInputConfig
 *	GameplayTag에 매핑된 일반 입력에 대한 InputAction과 Ability를 위한 InputAction들로 구성된 데이터 애셋.
 */
UCLASS(BlueprintType, Const)
class THEISLAND_API UTIInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UTIInputConfig(const FObjectInitializer& ObjectInitializer);

	// 주어진 태그에 해당되는 일반 입력용 InputAction을 리턴하는 함수.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	// 주어진 태그에 해당되는 Ability용 InputAction을 리턴하는 함수.
	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// GameplayTag와 매핑되는 일반 InputAction 배열.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTIInputAction> NativeInputActions;

	// GameplayTag와 매핑되는 Ability용 InputAction 배열.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTIInputAction> AbilityInputActions;
};
