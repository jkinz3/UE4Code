#pragma once
#include "FirstPersonCharacterMovement.generated.h"

UCLASS()

class UFirstPersonCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

		virtual float GetMaxSpeed() const override;

	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
};