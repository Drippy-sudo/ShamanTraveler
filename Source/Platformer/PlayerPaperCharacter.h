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

	void SetControlMode(bool Query);
	FORCEINLINE int GetControlIndex() { return ControlIndex; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRunningAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateJumpingAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ClampedVelocity;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsTouchingFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	bool IsInControlMode;

	bool IsWallSliding;

	float MoveValue;

	float PlayerTimeDialationVar;

	int ControlIndex;

	class APlatformerPlayerController* PlayerControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove;
};
