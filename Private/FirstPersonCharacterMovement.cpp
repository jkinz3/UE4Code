
#include "FirstPersonCharacterMovement.h"
#include "FirstPersonCharacter.h"


UFirstPersonCharacterMovement::UFirstPersonCharacterMovement( const FObjectInitializer& ObjectInitializer )
	:Super( ObjectInitializer )
{

}

float UFirstPersonCharacterMovement::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	AFirstPersonCharacter* FirstPersonCharacter = Cast<AFirstPersonCharacter>( PawnOwner );
	if ( FirstPersonCharacter )
	{
		if ( FirstPersonCharacter->IsSprinting() )
		{
			MaxSpeed *= FirstPersonCharacter->GetSprintScale();
			if (MovementMode == MOVE_Flying)
			{
				MaxSpeed *= FirstPersonCharacter->GetFlySprintScale();
			}
		}
	}

	return MaxSpeed;
}

void UFirstPersonCharacterMovement::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{
	Super::CalcVelocity(DeltaTime,  Friction, bFluid, BrakingDeceleration);

	if (MovementMode == MOVE_Flying)
	{
		if (Acceleration.IsZero())
		{
			Velocity = FVector::ZeroVector;
		}
		else
		{
			Velocity = GetMaxSpeed() * Acceleration.GetSafeNormal();
		}
	}
}