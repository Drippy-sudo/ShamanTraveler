// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Math/NumericLimits.h"
#include "Kismet/GameplayStatics.h"
#include "PlatformerPlayerController.h"

APlayerPaperCharacter::APlayerPaperCharacter()
{
	IsRunning = false;
	IsJumping = false;
	IsTouchingFront = false;
	IsInControlMode = false;
	IsWallSliding = false;
	CanMove = true;

	Points = 0;
	ControlIndex = 0;

	WallSlidingSpeed = 5.0f;
	MoveValue = 0.0f;
	ClampedVelocity = -300.0f;
	PlayerTimeDialationVar = 0.0f;
	Mana = 0.0f;
}

void APlayerPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerTimeDialationVar = this->GetActorTimeDilation();
	PlayerControllerRef = Cast<APlatformerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void APlayerPaperCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPaperCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ControlIndex", this, &APlayerPaperCharacter::ControlAxis);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void APlayerPaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAnimation();

	if (IsTouchingFront == true && GetMovementComponent()->IsMovingOnGround() == false && MoveValue != 0.0f)
	{
		IsWallSliding = true;
	}
	else
	{
		IsWallSliding = false;
	}

	if (IsWallSliding)
	{
		FVector Vel = GetVelocity();
		UCharacterMovementComponent* PlayerMovement = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (PlayerMovement != nullptr)
		{
			PlayerMovement->GravityScale = 0.0f;
			Vel.Z = FMath::Clamp(Vel.Z, ClampedVelocity, 0.0f);
		}
	}
	else
	{
		UCharacterMovementComponent* PlayerMovement = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (PlayerMovement != nullptr)
		{
			PlayerMovement->GravityScale = 1.0f;
		}
	}
}

void APlayerPaperCharacter::UpdateAnimation()
{
	if (CheckIfPlayerIsJumping())
	{
		CheckPlayerVelocity();
	}
	else
	{
		UpdateJumpingAnimation();
	}
}

bool APlayerPaperCharacter::CheckIfPlayerIsJumping()
{
	// Sets flipbook to jump and fall
	// Returns false when jumping
	// True when not
	if (GetMovementComponent()->Velocity.Z > 0.0f)
	{
		IsJumping = true;
		return false;
	}
	if (GetCharacterMovement()->IsFalling())
	{
		IsJumping = false;
		return false;
	}
	if (&ACharacter::OnLanded)
	{
		return true;
	}

	return true;
}

void APlayerPaperCharacter::CheckPlayerVelocity()
{
	FVector Velocity = this->GetVelocity();

	if (Velocity.Size() > 0.0f)
	{
		IsRunning = true;
	}
	else
	{
		IsRunning = false;
	}

	UpdateRunningAnimation();
}

void APlayerPaperCharacter::MoveRight(float Value)
{
	if (CanMove == false) return;

	if (IsWallSliding)
	{
		Value = 0.0f;
	}

	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);

	MoveValue = Value;
}

void APlayerPaperCharacter::ControlAxis(float Value)
{
	if (IsInControlMode == false) return;

	if (Value >= 0.5f && IsInControlMode)
	{
		// = 1
		// Up
		ControlIndex = 1;
	}
	else if (Value <= -0.5f && IsInControlMode)
	{
		// = -1
		// Down
		ControlIndex = -1;
	}
	else
	{
		// Reset
		ControlIndex = 0;
	}

	UE_LOG(LogTemp, Warning, TEXT("Control Index = %i"), ControlIndex);
}

void APlayerPaperCharacter::SetControlMode(bool Query)
{ 
	IsInControlMode = Query;

	if (IsInControlMode)
	{
		// Slow time dialation
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.7f);

		if (PlayerControllerRef)
			PlayerControllerRef->SetIsTimeSlow(true);
	}
	else
	{
		// Reset time dialation
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		if (PlayerControllerRef)
			PlayerControllerRef->SetIsTimeSlow(false);
	}
}
