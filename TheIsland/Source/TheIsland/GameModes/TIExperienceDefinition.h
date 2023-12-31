#pragma once

#include "Engine/DataAsset.h"
#include "TIExperienceDefinition.generated.h"

class UTIPawnData;

/*
* UTIExperienceDefinition
* 기존의 GameMode를 대신하여 게임의 매커니즘, 경험을 결정하는 개념인 Experience를 정의하는 클래스. Experience의 명세서.
*/
UCLASS(BlueprintType)
class UTIExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UTIExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	// Experince에서 활성화 하길 원하는 Game Feature 플러그인들의 리스트.
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	// Experience에서 플레이어와 AI가 조종할 기본 Pawn 정보가 담긴 클래스.
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<const UTIPawnData> DefaultPawnData;
};