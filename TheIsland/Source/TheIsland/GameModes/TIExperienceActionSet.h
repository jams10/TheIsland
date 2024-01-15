#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TIExperienceActionSet.generated.h"

class UGameFeatureAction;

/*
*	UTIExperienceActionSet
*	DataAsset���μ� UGameFeatureAction���� ���� ī�װ�ȭ ��ų �� ����� Ŭ����.
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