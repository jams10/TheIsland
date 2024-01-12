#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h" // UPrimaryDataAsset 상속을 위함.
#include "TIUserFacingExperienceDefinition.generated.h"

class UCommonSession_HostSessionRequest;

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

	// Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용하여, HostSessionRequest를 생성하는 함수.
	UFUNCTION(BlueprintCallable, BlueprintPure = false) // 실행 핀이 있도록 BlueprintPure 함수가 되지 않도록 함.
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;
};