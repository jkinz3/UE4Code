// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShake.h"
#include "Sound/SoundWave.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/SpotLightComponent.h"
#include "FirstPersonCharacterMovement.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Net/UnrealNetwork.h"
#include "FirstPersonCharacter.generated.h"

USTRUCT()
struct FImpactFX
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FVector_NetQuantize ImpactLocation;

	UPROPERTY()
		FVector_NetQuantizeNormal ImpactNormal;
};

UCLASS()
class JOHNCHARACTERS_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	virtual void BeginDestroy() override;

	virtual void Destroyed() override;



public:

	AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer);

public:

	virtual void BeginPlay() override;

	virtual void Tick( float DeltaTime ) override;

	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	void MoveForward( float Scale );
	void MoveRight( float Scale );
	void MoveUp(float Scale);
	void Turn( float Scale );
	void LookUp( float Scale );
	void TurnAtRate( float Scale );
	void LookUpAtRate( float Scale );
	void OnStartSprint();
	void OnStopSprint();
	void SetSprint( bool bNewSprintState );

	void OnFire();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerOnFire();


	UFUNCTION(Reliable, Server, WithValidation )
		void ServerSetSprint( bool bNewSprintState );

	UFUNCTION(BlueprintCallable)
		void SetZoom(bool bNewZoomState);

	void StartZoom();

	void StopZoom();

	void AdjustFOV(float DeltaTime);

	void PlayImpactEffects(FVector ImpactPoint, FVector ImpactNormal);

	
	
	float GetSprintScale();
	float GetFlySprintScale();

	void PickupObject(AActor* ActorToPickup, UPrimitiveComponent* ActorComponent);

	void OnUse();

	void OnQuit();
	void OnJump();
	virtual void Landed( const FHitResult& Hit ) override;
	void ToggleFlashlight();
	void Use();



	virtual void FellOutOfWorld(const class UDamageType& dmgType);

	UPROPERTY(ReplicatedUsing=OnRep_ImpactFX)
	FImpactFX ImpactData;


	UFUNCTION()
	void OnRep_ImpactFX();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		USceneComponent* HeldLocation;

protected:


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Camera" )
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Camera" )
		float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float  ZoomFOV;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float ZoomSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		bool bWantsToZoom;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement" )
		float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float FlySprintScale;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (EditCondition = "bEnableSprint") )
		float SprintScale;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps" )
		float RunStepRate;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps", meta = (ClampMin = ".001", EditCondition = "bEnableSprint") )
		float SprintStepRate;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps")
		bool bEnableSprint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Gameplay)
		float ThrowForce;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps" )
		TMap < TEnumAsByte<EPhysicalSurface>, USoundBase*> WalkStepsMap;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps", meta = (EditCondition = "bEnableSprint") )
		TMap < TEnumAsByte<EPhysicalSurface>, USoundBase*> SprintStepsMap;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps" )
		TMap <TEnumAsByte<EPhysicalSurface>, USoundBase*> LandStepsMap;

	UPROPERTY( VisibleDefaultsOnly, BlueprintReadWrite )
		UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		USpotLightComponent* Flashlight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		USkeletalMeshComponent* WorldMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Footsteps" )
		USoundBase* DefaultStepSound;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Lean", meta = (ClampMin = ".001", EditCondition = "bUseLean") )
		float LeanAmount;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Lean", meta = (EditCondition = "bUseLean") )
		float LeanSpeed;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay" )
		USoundWave* FlashlightSwitchSound;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Lean" )
		uint32 bEnableLean : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Gameplay)
		UParticleSystem* ImpactFX;

	void HandleFootsteps();

	UFUNCTION( BlueprintCallable )
		bool IsMovingForward();


public:

	UFUNCTION( BlueprintCallable )
		bool IsSprinting();

	UFUNCTION( BlueprintCallable )
		bool IsSprintEnabled();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category = Health)
		float PlayerHealth;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;

	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser);

	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser);

	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
		USoundBase* HurtSound;
private:

		int32 GetMaxHealth() const;

protected:

	FHitResult ForwardTrace();

	USoundBase* GetFootstepSound( EPhysicalSurface Surface, bool bIsJumping );

	void PlayFootstepSound( const FHitResult& DownHit, bool bIsJumping );

	FHitResult FootstepTrace();

	/* "Input" Strings*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString MoveForwardName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString MoveRightName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString LookUpName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString TurnName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString SprintName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString QuitName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString	JumpName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Input" )
		FString FlashlightName;

		uint8 bWantsToSprint : 1;

private:

	AActor* HeldActor;

	uint32 bIsMovingRight : 1;

	uint32 bIsMovingLeft : 1;

	uint32 bIsFlashlightOn : 1;

	float LastStepTime;

	float NextStepTime;

	float TargetLean;

	float DefaultLean;

	uint32 bIsHoldingObject : 1;

	UPhysicsHandleComponent* HoldingHandle;

	bool bIsSprinting;

	float CurrentFOV;
};