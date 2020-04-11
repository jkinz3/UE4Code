// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonCharacter.h"
#include "FirstPersonController.generated.h"


UCLASS()
class UEPROJECT_API AFirstPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;

	//virtual void SetPawn( APawn* InPawn );

	/*void MoveForward( float Scale );
	void MoveRight( float Scale );
	void Turn( float Scale );
	void LookUp( float Scale );
	void TurnRate( float Scale );
	void LookUpRate( float Scale );
	void OnStartSprint();
	void OnStopSprint();

	void OnQuit();

	void OnUse();
	void OnJump();
	void ToggleFlashlight();

private:

	UPROPERTY()
		AFirstPersonCharacter* FirstPersonCharacter;*/
};
