// Fill out your copyright notice in the Description page of Project Settings.

#include "LiftableBlock.h"

#include "Projectile.h"
#include "PlayerPaperCharacter.h"

#include "TimerManager.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Kismet/GameplayStatics.h"

/*
* A interactable block that when hit you can move it up only and it stops the player moving.
* If moved to a certain height will move slowly downwards, so setting the time dialation to move slower.
* Else it will just drop back down to the ground like a physics object and kill enemies.
*/
ALiftableBlock::ALiftableBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	ControlTime = 10.0f;
	Distance = 200.0f;
	Speed = 2.0f;
	SlowFallTimeDialation = 0.1f;

	bIsTargeted = false;
	bIsInSlowFallMode = false;
}

void ALiftableBlock::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World == nullptr) return;

	BeginLocation = GetActorLocation();

	TargetLocation = FVector(0.0f, 0.0f, GetActorLocation().Z + 300.0f);

	PlayerRef = Cast<APlayerPaperCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ALiftableBlock::OnOverlapBegin);
}

void ALiftableBlock::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerRef == nullptr)
	{
		return;
	}

	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile == nullptr) 
	{
		return;
	}

	bIsTargeted = true;

	// Set player in control mode
	PlayerRef->SetControlMode(true);

	InControlMode();

	// Set 2 second timer for when it ends
	GetWorldTimerManager().SetTimer(ControlTimerHandle, this, &ALiftableBlock::ResetPlayersControlMode, ControlTime, false);

	UE_LOG(LogTemp, Warning, TEXT("Test"));
}

void ALiftableBlock::ResetPlayersControlMode()
{
	if (PlayerRef == nullptr)
	{
		UWorld* World = GetWorld();

		if (World == nullptr) return;

		PlayerRef = Cast<APlayerPaperCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	}

	PlayerRef->SetControlMode(false);
	bIsTargeted = false;
	OutControlMode();
	GetWorldTimerManager().ClearTimer(ControlTimerHandle);
}

void ALiftableBlock::Move(float DeltaTime)
{
	FVector StartLocation = GetActorLocation();

	SetActorRelativeLocation(FMath::Lerp(StartLocation, DesiredLocation, Speed * DeltaTime));
}

void ALiftableBlock::SetSlowFallMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Entering slow fall mode.."));
	StartFallLocation = GetActorLocation();
	ResetPlayersControlMode();
	//this->CustomTimeDilation = SlowFallTimeDialation;
	StaticMeshComp->SetSimulatePhysics(true);
	bIsInSlowFallMode = true;
}

void ALiftableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static bool Switch = false;

	if (bIsTargeted 
		&& PlayerRef->GetControlIndex() == 1.0f)
	{
		Switch = true;
		if (GetActorLocation().Z <= TargetLocation.Z)
		{

			DesiredLocation = GetActorLocation();
			DesiredLocation.Z += Distance;
			Move(DeltaTime);
		}
		else
		{
			SetSlowFallMode();
		}
	}

	if (PlayerRef->GetControlIndex() == 0.0f && Switch == true)
	{
		SetSlowFallMode();
		Switch = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetActorLocation().Z);
}
