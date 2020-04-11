//
//
//#pragma once
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "FPSCharacter.generated.h"
//
//class UInputComponent;
//class UCharacterMovementComponent;
//
//UCLASS(config = Game)
//class AFPSCharacter : public ACharacter
//{
//	GENERATED_BODY()
//
//		/** Pawn mesh: 1st person view (arms; seen only by self) */
//		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
//		class USkeletalMeshComponent* Mesh1P;
//
//	/** Gun mesh: 1st person view (seen only by self) */
//	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
//		class USkeletalMeshComponent* FP_Gun;
//
//	/** First person camera */
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
//		class UCameraComponent* FirstPersonCameraComponent;
//
//public:
//
//	AFPSCharacter();
//
//protected:
//
//	virtual void BeginPlay();
//
//	/** Gun muzzle's offset from the characters location */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
//		FVector GunOffset;
//
//	/** Sound to play each time we fire */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
//		class USoundBase* FireSound;
//
//	/** AnimMontage to play each time we fire */
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
//		class UAnimMontage* FireAnimation;
//
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
//		float WeaponRange;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
//		float WeaponDamage;
//
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
//		UParticleSystem* ImpactFX;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
//		float SprintSpeed;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
//		float WalkSpeed;
//};