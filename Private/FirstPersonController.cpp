#include "..\Public\FirstPersonController.h"
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

void AFirstPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis( "MoveForward", this, &AFirstPersonPlayerController::MoveForward );
	//InputComponent->BindAxis( "MoveRight", this, &AFirstPersonPlayerController::MoveRight );
	//InputComponent->BindAxis( "Turn", this, &AFirstPersonPlayerController::Turn );
	//InputComponent->BindAxis( "LookUp", this, &AFirstPersonPlayerController::LookUp );
	//InputComponent->BindAxis( "TurnRate", this, &AFirstPersonPlayerController::TurnRate );
	//InputComponent->BindAxis( "LookUpRate", this, &AFirstPersonPlayerController::LookUpRate );

	//InputComponent->BindAction( "Sprint", IE_Pressed, this, &AFirstPersonPlayerController::OnStartSprint );
	//InputComponent->BindAction( "Sprint", IE_Released, this, &AFirstPersonPlayerController::OnStopSprint );

	//InputComponent->BindAction( "Jump", IE_Pressed, this, &AFirstPersonPlayerController::OnJump );
	//InputComponent->BindAction( "Quit", IE_Pressed, this, &AFirstPersonPlayerController::OnQuit );
	//InputComponent->BindAction( "Use", IE_Pressed, this, &AFirstPersonPlayerController::OnUse );

	//InputComponent->BindAction( "Flashlight", IE_Pressed, this, &AFirstPersonPlayerController::ToggleFlashlight );
}

//void AFirstPersonPlayerController::SetPawn( APawn * InPawn )
//{
//	AController::SetPawn( InPawn );
////	FirstPersonCharacter = Cast<AFirstPersonCharacter>( InPawn );
//}
//void AFirstPersonPlayerController::MoveForward( float Scale )
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->MoveForward( Scale );
//	}
//}
//
//void AFirstPersonPlayerController::MoveRight( float Scale )
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->MoveRight( Scale );
//	}
//}
//
//void AFirstPersonPlayerController::Turn( float Scale )
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->Turn( Scale );
//	}
//}
//
//void AFirstPersonPlayerController::LookUp( float Scale )
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->LookUp( Scale );
//	}
//}
//
//void AFirstPersonPlayerController::TurnRate( float Scale )
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->TurnAtRate( Scale );
//	}
//}
//
//void AFirstPersonPlayerController::LookUpRate( float Scale )
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->LookUpAtRate( Scale );
//	}
//}
//
//void AFirstPersonPlayerController::OnStartSprint()
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->OnStartSprint();
//	}
//}
//
//void AFirstPersonPlayerController::OnStopSprint()
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->OnStopSprint();
//	}
//}
//
//void AFirstPersonPlayerController::OnQuit()
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->OnQuit();
//	}
//}
//
//void AFirstPersonPlayerController::OnUse()
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->Use();
//	}
//}
//
//void AFirstPersonPlayerController::OnJump()
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->OnJump();
//	}
//}
//
//void AFirstPersonPlayerController::ToggleFlashlight()
//{
//	if ( FirstPersonCharacter != nullptr )
//	{
//		FirstPersonCharacter->ToggleFlashlight();
//	}
//}
