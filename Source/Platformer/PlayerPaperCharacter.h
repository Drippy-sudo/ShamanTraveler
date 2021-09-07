// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PlayerPaperCharacter.generated.h"

//class USceneComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class PLATFORMER_API APlayerPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	void MoveRight(float Value);
	void ControlAxis(float Value);
	void CheckPlayerVelocity();
	bool CheckIfPlayerIsJumping();
	void UpdateAnimation();
	void Ultimate();

	void SetControlMode(bool Query);
	FORCEINLINE int GetControlIndex() { return ControlIndex; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* UltimateSphereComp;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRunningAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateJumpingAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnUltimateFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ClampedVelocity;

	void SwitchOffUltimateCollision();

	FVector GetRollingResistance();

public:
	APlayerPaperCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
	int Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WallSlidingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ultimate")
	float UltimateDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsTouchingFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool IsInControlMode;

	bool IsWallSliding;

	float MoveValue;

	float PlayerTimeDialationVar;

	int ControlIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Mana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ManaCap = 100.0f;

	class APlatformerPlayerController* PlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove;

	UPROPERTY(EditDefaultsOnly)
	float GravityScaleWhilstWallSliding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Mass;

	// Higher means more rolling resistance.
	UPROPERTY(EditDefaultsOnly)
	float RollingResistanceCoefficient = 0.015f;

	UPROPERTY(EditAnywhere)
	float MaxMovingForce = 100.0f;

	UCharacterMovementComponent* PlayerMovementComp;
	UWorld* World;
	FTimerHandle UltimateTimerHandle;
};
