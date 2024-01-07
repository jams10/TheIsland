#pragma once
#include "TICameraMode.h"
#include "TICameraMode_ThirdPerson.generated.h"

/*
*	UTICameraMode_ThirdPerson
*	3인칭 카메라 모드.
*/
UCLASS(Abstract, Blueprintable)
class UTICameraMode_ThirdPerson : public UTICameraMode
{
	GENERATED_BODY()
public:
	UTICameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};