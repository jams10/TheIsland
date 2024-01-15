#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TIExperienceActionSet.generated.h"

class UGameFeatureAction;

/*
*	UTIExperienceActionSet
*	DataAsset으로서 UGameFeatureAction들을 묶어 카테고리화 시킬 때 사용할 클래스.
*/
UCLASS(BlueprintType)
class UTIExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UTIExperienceActionSet();

public:
	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};