#pragma once
#include "TICameraMode.h"
#include "TICameraMode_ThirdPerson.generated.h"

class UCurveVector;

/*
*	UTICameraMode_ThirdPerson
*	3인칭 카메라 모드.
*/
UCLASS(Abstract, Blueprintable)
class UTICameraMode_ThirdPerson : public UTICameraMode
{
	GENERATED_BODY()
public:
	UTICameraMode_ThirdPerson();

	/*
		UTICameraMode's Interface
	*/
	virtual void UpdateView(float DeltaTime) override;

	/*
		member variables
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};