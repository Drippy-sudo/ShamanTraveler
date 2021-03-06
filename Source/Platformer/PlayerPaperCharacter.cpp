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
#include "Components/SphereComponent.h"

APlayerPaperCharacter::APlayerPaperCharacter()
{
	UltimateSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Ultimate Sphere Comp"));
	UltimateSphereComp->SetupAttachment(RootComponent);

	UltimateSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	UltimateDuration = 0.5f;
	GravityScaleWhilstWallSliding = 2.0f;
	Mass = 100.0f;
}

void APlayerPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerTimeDialationVar = this->GetActorTimeDilation();
	PlayerControllerRef = Cast<APlatformerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerMovementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	World = GetWorld();
}

void APlayerPaperCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPaperCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ControlIndex", this, &APlayerPaperCharacter::ControlAxis);

	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &APlayerPaperCharacter::Ultimate);
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
		PlayerMovementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (PlayerMovementComp != nullptr)
		{
			PlayerMovementComp->GravityScale = 0.0f;
			Vel.Z = FMath::Clamp(Vel.Z, ClampedVelocity, 0.0f);
		}
	}
	else
	{
		PlayerMovementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (PlayerMovementComp != nullptr)
		{
			PlayerMovementComp->GravityScale = GravityScaleWhilstWallSliding;
		}
	}
}

void APlayerPaperCharacter::Ultimate()
{
	if (Mana >= ManaCap)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pressed ultimate"));

		UltimateSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		SpawnUltimateFX();

		GetWorldTimerManager().SetTimer(UltimateTimerHandle, this, &APlayerPaperCharacter::SwitchOffUltimateCollision, UltimateDuration, false);
	}
}

void APlayerPaperCharacter::SwitchOffUltimateCollision()
{
	UltimateSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mana = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Ultimate worn off"));
}

FVector APlayerPaperCharacter::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -GetVelocity().GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
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
	if (CanMove == false)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
			DisableInput(PlayerController);

		return;
	}

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

		UE_LOG(LogTemp, Warning, TEXT("In Control Mode..."));
	}
	else
	{
		// Reset time dialation
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

		if (PlayerControllerRef)
			PlayerControllerRef->SetIsTimeSlow(false);

		UE_LOG(LogTemp, Warning, TEXT("Out Control Mode..."));
	}
}
