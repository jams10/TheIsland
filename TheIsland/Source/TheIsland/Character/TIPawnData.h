#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TIPawnData.generated.h"

UCLASS(BlueprintType)
class UTIPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTIPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Spawn�ؼ� ������ Pawn Ŭ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TI|Pawn")
	TSubclassOf<APawn> PawnClass;
};