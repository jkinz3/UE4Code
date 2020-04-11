#include "..\Public\FirstPersonCharacter.h"
#include "FPGameMode.h"
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


void AFirstPersonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetLocalRole() == ROLE_Authority)
	{
		PlayerHealth = GetMaxHealth();
	}
	 

}

void AFirstPersonCharacter::BeginDestroy()
{
	Super::BeginDestroy();

}

void AFirstPersonCharacter::Destroyed()
{
	Super::Destroyed(); 
}

AFirstPersonCharacter::AFirstPersonCharacter( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer.SetDefaultSubobjectClass<UFirstPersonCharacterMovement>( ACharacter::CharacterMovementComponentName ) )
{
	PrimaryActorTick.bCanEverTick = true;

	

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "PlayerCamera" ) );
	PlayerCamera->SetupAttachment( RootComponent );
	PlayerCamera->SetRelativeLocation ( FVector( 0.f, 0.f, 64.f ));
	PlayerCamera->bUsePawnControlRotation = true;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT( "Flashlight" ));
	Flashlight->SetupAttachment( PlayerCamera );
	Flashlight->SetRelativeLocation(FVector( 0.f, 0.f, 4.f ));
	Flashlight->SetVisibility( false );
	
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	WorldMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "3rd Person Mesh" ) );
	WorldMesh->SetupAttachment( RootComponent );
	WorldMesh->SetRelativeLocation(FVector( 0.f, 0.f, -90.f ));
	WorldMesh->SetRelativeRotation (FRotator( 0.f, 0.f, -90.f ));

	HeldLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Held Location"));
	HeldLocation->SetupAttachment(PlayerCamera);

	bEnableSprint = false;
	RunSpeed = 600.f;
	FlySprintScale = 7.f;
	SprintScale = 1.7f;
	bIsFlashlightOn = false;
	RunStepRate = .6; 
	SprintStepRate = .3f;
	DefaultLean = 0.f;
	LeanAmount = .001f;
	LeanSpeed = 20.f;
	bEnableLean = false;

	bWantsToSprint = false;

	/*inputs*/

	MoveForwardName = TEXT( "Moveforward" );
	MoveRightName = TEXT( "MoveRight" );
	TurnName = TEXT( "Turn" );
	LookUpName = TEXT( "LookUp" );
	JumpName = TEXT( "Jump" );
	SprintName = TEXT( "Sprint" );
	QuitName = TEXT( "Quit" );
	FlashlightName = TEXT( "Flashlight" );

	GetCharacterMovement()->bCheatFlying = true;
	GetCharacterMovement()->MaxFlySpeed = 1500.f;

	ThrowForce = 100000.f;


	BaseFOV = 90.f;
	ZoomFOV = 40.f;

	CurrentFOV = BaseFOV;
	ZoomSpeed = 5.f;

	
}

void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AFirstPersonCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsHoldingObject && HoldingHandle != nullptr)
	{
		HoldingHandle->SetTargetLocation(HeldLocation->GetComponentLocation());
		FRotator NewRotation = FRotator::ZeroRotator;
		NewRotation.Yaw = GetActorRotation().Yaw;

		HeldActor->SetActorRotation(NewRotation);
	}

	AdjustFOV(DeltaTime);
}

void AFirstPersonCharacter::SetupPlayerInputComponent( UInputComponent * PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( *MoveForwardName, this, &AFirstPersonCharacter::MoveForward );
	PlayerInputComponent->BindAxis( *MoveRightName, this, &AFirstPersonCharacter::MoveRight );
	PlayerInputComponent->BindAxis("MoveUp", this, &AFirstPersonCharacter::MoveUp);
	PlayerInputComponent->BindAxis( *LookUpName, this, &AFirstPersonCharacter::LookUp );
	PlayerInputComponent->BindAxis( *TurnName, this, &AFirstPersonCharacter::Turn );
	PlayerInputComponent->BindAction( *JumpName, IE_Pressed, this, &AFirstPersonCharacter::OnJump );
	PlayerInputComponent->BindAction( *SprintName, IE_Pressed, this, &AFirstPersonCharacter::OnStartSprint );
	PlayerInputComponent->BindAction( *SprintName, IE_Released, this, &AFirstPersonCharacter::OnStopSprint );
	PlayerInputComponent->BindAction( *QuitName, IE_Pressed, this, &AFirstPersonCharacter::OnQuit );
	PlayerInputComponent->BindAction( *FlashlightName, IE_Pressed, this, &AFirstPersonCharacter::ToggleFlashlight );
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFirstPersonCharacter::OnFire);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFirstPersonCharacter::StartZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFirstPersonCharacter::StopZoom);
	

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AFirstPersonCharacter::OnUse);
}

void AFirstPersonCharacter::MoveForward( float Scale )
{
	if ( Scale == 0.f )
	{
		return;
	}
	if ( GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying )
	{
		AddMovementInput( GetControlRotation().Vector(), Scale );
	}
	AddMovementInput( GetActorForwardVector(), Scale );

	HandleFootsteps();
}

void AFirstPersonCharacter::MoveRight( float Scale )
{
	if ( Scale == 0.f )
	{
		return;
	}
	AddMovementInput( GetActorRightVector(), Scale );

	HandleFootsteps();
}

void AFirstPersonCharacter::MoveUp(float Scale)
{
	if (GetCharacterMovement()->MovementMode != MOVE_Flying || Scale == 0.f)
	{
		return;
	}
	AddMovementInput(GetActorUpVector(), Scale);
}

void AFirstPersonCharacter::Turn( float Scale )
{
	if ( Scale == 0.f )
	{
		return;
	}
	AddControllerYawInput( Scale );
}

void AFirstPersonCharacter::LookUp( float Scale )
{
	if ( Scale == 0.f )
	{
		return;
	}
	AddControllerPitchInput( Scale );
	/*	FRotator CameraRotation = PlayerCamera->GetRelativeRotation();
	FRotator ControlRotation = GetController()->GetControlRotation();
	FRotator NewRotation = FRotator(ControlRotation.Pitch, ControlRotation.Yaw, ControlRotation.Roll);
	PlayerCamera->SetRelativeRotation(NewRotation);*/
}

void AFirstPersonCharacter::TurnAtRate( float Scale )
{
	if ( Scale == 0.f )
	{
		return;
	}
	AddControllerYawInput( Scale* BaseTurnRate * GetWorld()->GetDeltaSeconds() );

}

void AFirstPersonCharacter::LookUpAtRate( float Scale )
{
	if ( Scale == 0.f )
	{
		return;
	}
	AddControllerPitchInput( Scale * BaseLookUpRate * GetWorld()->GetDeltaSeconds() );
}

void AFirstPersonCharacter::OnStartSprint()
{
	SetSprint( true );
}

void AFirstPersonCharacter::OnStopSprint()
{
	SetSprint( false );
}

void AFirstPersonCharacter::SetSprint( bool bNewSprintState )
{
	if (bEnableSprint)
	{
		bWantsToSprint = bNewSprintState;
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerSetSprint(bNewSprintState);
		}
	}
}

void AFirstPersonCharacter::OnFire()
{

	if (bIsHoldingObject)
	{
		HoldingHandle->ReleaseComponent();
		bIsHoldingObject = false;
		UPrimitiveComponent* MeshToThrow = Cast<UPrimitiveComponent>(HeldActor->GetRootComponent());
		if (MeshToThrow != nullptr)
		{
			FVector ThrowVector = GetControlRotation().Vector() * ThrowForce;
			MeshToThrow->AddImpulse(ThrowVector);
		}
		
	}

	if (GetLocalRole() < ROLE_Authority)
	{
		ServerOnFire();
		
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector ShootDir = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;

	if (PlayerController)
	{
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(StartTrace, CamRot);
		ShootDir = CamRot.Vector();
	}

	const FVector EndTrace = StartTrace + ShootDir * 5000.f;

	FCollisionQueryParams TraceParams;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Impact;
	GetWorld()->LineTraceSingleByChannel(Impact, StartTrace, EndTrace, ECC_Visibility, TraceParams);

	AActor* DamagedActor = Impact.GetActor();
	UPrimitiveComponent* DamagedComponent = Impact.GetComponent();

	PlayImpactEffects(Impact.ImpactPoint, Impact.ImpactNormal);

	if (GetLocalRole() == ROLE_Authority)
	{
		ImpactData.ImpactLocation = Impact.ImpactPoint;
		ImpactData.ImpactNormal = Impact.ImpactNormal;
	}
	
}

void AFirstPersonCharacter::ServerOnFire_Implementation()
{
	OnFire();
}

bool AFirstPersonCharacter::ServerOnFire_Validate()
{
	return true;
}

bool AFirstPersonCharacter::IsSprinting()
{
	if ( !GetCharacterMovement() )
	{
		return false;
	}
	if (GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		return IsMovingForward() && (bWantsToSprint) && !GetVelocity().IsZero() && (GetVelocity().GetSafeNormal2D() | GetActorForwardVector()) > -.1f;
	}
	else
	{
		return bWantsToSprint;
	}
}

bool AFirstPersonCharacter::ServerSetSprint_Validate( bool bNewSprintState )
{
	return true;
}

void AFirstPersonCharacter::ServerSetSprint_Implementation( bool bNewSprintState )
{
	SetSprint( bNewSprintState );
}



void AFirstPersonCharacter::SetZoom(bool bNewZoomState)
{
	bWantsToZoom = bNewZoomState;
}

void AFirstPersonCharacter::StartZoom()
{
	SetZoom(true);
}

void AFirstPersonCharacter::StopZoom()
{
	SetZoom(false);
}

void AFirstPersonCharacter::AdjustFOV(float DeltaTime)
{
	float TargetFOV = BaseFOV;

	if (bWantsToZoom)
	{
		TargetFOV = ZoomFOV;
	}
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomSpeed);
	CurrentFOV = NewFOV;
	PlayerCamera->SetFieldOfView(NewFOV);
	 
}

void AFirstPersonCharacter::PlayImpactEffects(FVector ImpactPoint, FVector ImpactNormal)
{

	if (ImpactFX)
	{
	
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, ImpactPoint, ImpactNormal.Rotation());
	}
}

float AFirstPersonCharacter::GetSprintScale()
{
	return SprintScale;
}

float AFirstPersonCharacter::GetFlySprintScale()
{
	return FlySprintScale;
}

void AFirstPersonCharacter::PickupObject(AActor* ActorToPickup, UPrimitiveComponent* ActorComponent)
{
	if (!bIsHoldingObject)
	{
		UPhysicsHandleComponent* PhysicsHandleComponent = NewObject<UPhysicsHandleComponent>(ActorToPickup);
		PhysicsHandleComponent->RegisterComponent();

		HoldingHandle = PhysicsHandleComponent;
		HoldingHandle = PhysicsHandleComponent;

		PhysicsHandleComponent->GrabComponentAtLocation(ActorComponent, TEXT("None"), ActorToPickup->GetActorLocation());

		bIsHoldingObject = true;

		HeldActor = ActorToPickup;
	}
	else
	{
		if (HoldingHandle != nullptr)
		{
			HoldingHandle->ReleaseComponent();
			bIsHoldingObject = false;
		}
	}
}

void AFirstPersonCharacter::OnUse()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	FVector ShootDir = FVector::ZeroVector;
	FVector StartTrace = FVector::ZeroVector;

	if (PlayerController)
	{
		FRotator CamRot;
		PlayerController->GetPlayerViewPoint(StartTrace, CamRot);
		ShootDir = CamRot.Vector();
	}

	const FVector EndTrace = StartTrace + ShootDir * 300.f;

	FCollisionQueryParams TraceParams;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, TraceParams);

	HeldActor = Hit.Actor.Get();
	
	if (Hit.Actor != NULL && Hit.Component != NULL)
	{
		PickupObject(Hit.Actor.Get(), Hit.Component.Get());
	}


}

void AFirstPersonCharacter::OnQuit()
{
}

void AFirstPersonCharacter::OnJump()
{
	if ( GetCharacterMovement()->IsMovingOnGround() )
	{
		Jump();
		FHitResult DownHit = FootstepTrace();

		PlayFootstepSound( DownHit, true );

	}
}

void AFirstPersonCharacter::Landed( const FHitResult & Hit )
{
	Super::Landed( Hit );
	PlayFootstepSound( Hit, false );

}

void AFirstPersonCharacter::ToggleFlashlight()
{


		Flashlight->SetVisibility( !bIsFlashlightOn );

		bIsFlashlightOn = !bIsFlashlightOn;

		if (FlashlightSwitchSound != nullptr)
		{
			UGameplayStatics::SpawnSoundAttached(FlashlightSwitchSound, GetRootComponent());

		}

}

void AFirstPersonCharacter::Use()
{
}

void AFirstPersonCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	Super::FellOutOfWorld(dmgType);
}

void AFirstPersonCharacter::OnRep_ImpactFX()
{
	//play cosmetiv FX

	PlayImpactEffects(ImpactData.ImpactLocation, ImpactData.ImpactNormal );

}

void AFirstPersonCharacter::HandleFootsteps()
{
	if ( UGameplayStatics::GetRealTimeSeconds( GetWorld() ) >= NextStepTime && GetCharacterMovement()->IsMovingOnGround() )
	{
		FHitResult FootstepHitResult = FootstepTrace();

		PlayFootstepSound( FootstepHitResult, false );

		float CurrentTime = UGameplayStatics::GetRealTimeSeconds( GetWorld() );

		LastStepTime = CurrentTime;

		float TimeBetweenSteps = RunStepRate;

		if (IsSprinting())
		{
			TimeBetweenSteps = SprintStepRate;
		}

		NextStepTime = CurrentTime + TimeBetweenSteps;
	}
}

bool AFirstPersonCharacter::IsMovingForward()
{
	FVector ForwardVector = GetActorForwardVector();
	FVector Velocity = GetVelocity();
	float DotProduct = FVector::DotProduct(ForwardVector, Velocity);

	if (DotProduct > .7f)
	{
		return true;
	}

	return false;

}


bool AFirstPersonCharacter::IsSprintEnabled()
{
	FVector PlayerVelocity = GetVelocity();
	FVector ForwardVector = GetActorForwardVector();
	float DotProduct = FVector::DotProduct( PlayerVelocity, ForwardVector );
	return (DotProduct > .7f);
}
float AFirstPersonCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	AFirstPersonCharacter* MyPC = Cast<AFirstPersonCharacter>(Controller);

	if (PlayerHealth <= 0.0f)
	{
		return 0.f;
	}
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		PlayerHealth -= ActualDamage;
		if (PlayerHealth <= 0)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{
			PlayHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		}
	}
	return ActualDamage;
}

bool AFirstPersonCharacter::CanDie(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser) const
{
	if (IsPendingKill()
		|| GetLocalRole() != ROLE_Authority)
	{
		return false;
	}
	return true;
}

bool AFirstPersonCharacter::Die(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return false;
	}
	PlayerHealth = FMath::Min(0.0f, PlayerHealth);
	
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);

	AController* const KilledPlayer = (Controller != NULL) ? Controller : Cast<AController>(GetOwner());
	GetCharacterMovement()->ForceReplicationUpdate();

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);
	return true;
}

void AFirstPersonCharacter::OnDeath(float KillingDamage, FDamageEvent const & DamageEvent, APawn * InstigatingPawn, AActor * DamageCauser)
{
	SetReplicateMovement(false);
	TearOff();

	if (GetLocalRole() == ROLE_Authority)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC && DamageEvent.DamageTypeClass)
		{
			
		}
	}
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	APlayerController* PC = Cast<APlayerController>(GetController());

	GetWorld()->GetAuthGameMode<AFPGameMode>()->RespawnPlayer(PC);
}

void AFirstPersonCharacter::PlayHit(float DamageTaken, FDamageEvent const & DamageEvent, APawn * PawnInstigator, AActor * DamageCauser)
{

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
		UGameplayStatics::SpawnSoundAttached(HurtSound, GetRootComponent());
	}


}

int32 AFirstPersonCharacter::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<AFirstPersonCharacter>()->PlayerHealth;
}

FHitResult AFirstPersonCharacter::ForwardTrace()
{
	FHitResult RV_ForwardHit;
	static FName NAME_ForwardTrace( TEXT( "Forward Trace" ) );
	FCollisionQueryParams QueryParams( NAME_ForwardTrace, false, this );

	QueryParams.TraceTag = NAME_ForwardTrace;

	const FVector TraceStart = PlayerCamera->GetComponentLocation();
	const FVector TraceEnd = TraceStart + GetControlRotation().Vector() * 1000.f;

	GetWorld()->LineTraceSingleByChannel( RV_ForwardHit, TraceStart, TraceEnd, GetCapsuleComponent()->GetCollisionObjectType(), QueryParams );
	return RV_ForwardHit;
}

USoundBase * AFirstPersonCharacter::GetFootstepSound( EPhysicalSurface Surface, bool bIsJumping )
{
	USoundBase** SoundPtr = nullptr;

	if ( IsSprintEnabled() )
	{

		if ( IsSprinting() )
		{
			SoundPtr = SprintStepsMap.Find( Surface );
		}
		else if ( !IsSprinting() )
		{
			SoundPtr = WalkStepsMap.Find( Surface );
		}
		if ( bIsJumping )
		{
			SoundPtr = SprintStepsMap.Find( Surface );
		}
		if ( !bIsJumping && GetCharacterMovement()->IsFalling() )
		{
			SoundPtr = LandStepsMap.Find( Surface );
		}
	}
	else
	{
		if ( !bIsJumping && GetCharacterMovement()->IsFalling() )
		{
			SoundPtr = LandStepsMap.Find( Surface );
		}
		SoundPtr = SprintStepsMap.Find( Surface );
	}
	return SoundPtr ? *SoundPtr : nullptr;
}

void AFirstPersonCharacter::PlayFootstepSound( const FHitResult & DownHit, bool bIsJumping )
{
	if ( DownHit.PhysMaterial != NULL )
	{
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType( DownHit.PhysMaterial.Get() );

		USoundBase* NewFootstepSound = GetFootstepSound( SurfaceType, bIsJumping );

		if ( NewFootstepSound )
		{
			UGameplayStatics::SpawnSoundAtLocation( this, NewFootstepSound, DownHit.Location );
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation( this, DefaultStepSound, DownHit.Location );
		}
	}
	else
	{
		UGameplayStatics::SpawnSoundAtLocation(this, DefaultStepSound, DownHit.Location);
	}
}

FHitResult AFirstPersonCharacter::FootstepTrace()
{
	static FName NAME_FootstepTrace( TEXT( "FootstepTrace" ) );

	FCollisionQueryParams QueryParams( NAME_FootstepTrace, false, this );

	QueryParams.bReturnPhysicalMaterial = true;

	float PawnRadius;

	float PawnHalfHeight;

	GetCapsuleComponent()->GetScaledCapsuleSize( PawnRadius, PawnHalfHeight );

	const FVector LineTraceStart = GetCapsuleComponent()->GetComponentLocation();

	const float TraceDistance = PawnHalfHeight + 10.0f;

	const FVector DownDirection = FVector( 0.0f, 0.0f, (-TraceDistance - 200.0f) );

	FHitResult RV_DownHit;
	
	GetWorld()->LineTraceSingleByChannel( RV_DownHit, LineTraceStart, LineTraceStart + DownDirection, GetCapsuleComponent()->GetCollisionObjectType(), QueryParams );

	return RV_DownHit;
}

void AFirstPersonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFirstPersonCharacter, ImpactData, COND_SkipOwner);
	DOREPLIFETIME(AFirstPersonCharacter, PlayerHealth);
}