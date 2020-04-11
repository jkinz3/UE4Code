// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class JOHNCHARACTERS_API AFPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void RespawnPlayer(APlayerController* PlayerToRespawn);
	
};
