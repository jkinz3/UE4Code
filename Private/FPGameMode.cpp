// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"

void AFPGameMode::RespawnPlayer(APlayerController * PlayerToRespawn)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn() == nullptr)
		{
			RestartPlayer(PC);
		}
	}
}
