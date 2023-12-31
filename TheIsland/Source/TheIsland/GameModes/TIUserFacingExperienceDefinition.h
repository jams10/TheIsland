#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h" // UPrimaryDataAsset 상속을 위함.
#include "TIUserFacingExperienceDefinition.generated.h"

/*
* TIUserFacingExperienceDefinition
* Experience를 적용하기 위한 진입점. Experience가 적용될 Map, 적용할 Experience의 정보가 담긴 ExperienceDefinition을 담고 있음.
*/
UCLASS(BlueprintType)
class UTIUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 게임 모드에 따라 불러올 맵의 애셋 ID.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	// 불러올 게임 플레이 experience, 게임 모드의 ID.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "TIExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};