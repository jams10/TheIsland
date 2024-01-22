#pragma once

#include "GameFramework/Actor.h"
#include "TITaggedActor.generated.h"

UCLASS()
class ATITaggedActor : public AActor
{
	GENERATED_BODY()

public:
	ATITaggedActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};