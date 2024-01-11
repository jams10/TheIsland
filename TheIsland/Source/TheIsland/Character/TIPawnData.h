#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TIPawnData.generated.h"

class UTICameraMode;
class UTIInputConfig;

UCLASS(BlueprintType)
class UTIPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTIPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Spawn해서 조종할 Pawn 클래스.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TI|Pawn")
	TSubclassOf<APawn> PawnClass;

	// 카메라 모드
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TI|Camera")
	TSubclassOf<UTICameraMode> DefaultCameraMode;

	// Input Config 애셋.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TI|InputConfig")
	TObjectPtr<UTIInputConfig> InputConfig;
};