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

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTIInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTIInputAction> AbilityInputActions;
};
