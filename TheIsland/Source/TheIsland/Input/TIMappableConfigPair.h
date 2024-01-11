#pragma once

#include "TIMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FTIMappableConfigPair
{
	GENERATED_BODY()

public:
	FTIMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};
