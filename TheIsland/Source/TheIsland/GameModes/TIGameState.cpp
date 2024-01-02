#include "TIGameState.h"
#include "TIExperienceManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIGameState)

ATIGameState::ATIGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UTIExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
