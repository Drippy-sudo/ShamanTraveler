// Fill out your copyright notice in the Description page of Project Settings.

#include "VerticleSliderPlatform.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Projectile.h"
#include "PlayerPaperCharacter.h"
#include "TimerManager.h"
#include "Math/UnrealMathVectorCommon.h"

AVerticleSliderPlatform::AVerticleSliderPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	RootComponent = RootComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	CanMove = false;
	WasHit = false;
	Distance = 200.0f;
	Speed = 2.0f;
	ControlTime = 2.0f;
}

void AVerticleSliderPlatform::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AVerticleSliderPlatform::OnOverlapBegin);

	PlayerRef = Cast<APlayerPaperCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerRef == nullptr) return;
}

void AVerticleSliderPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove)
	{
		switch (PlayerRef->GetControlIndex())
		{
		case 1:
			EndLocation = GetActorLocation();
			EndLocation.Z += Distance;
			Move(DeltaTime);
			break;

		case -1:
			EndLocation = GetActorLocation();
			EndLocation.Z -= Distance;
			Move(DeltaTime);
			break;

		default:
			// Do nothing
			break;
		}
	}
}

void AVerticleSliderPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PlayerRef == nullptr)
	{
		PlayerRef = Cast<APlayerPaperCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile == nullptr) return;

	//Projectile->Destroy();

	WasHit = true;
	CanMove = true;

	// Set player in control mode
	PlayerRef->SetControlMode(true);

	InControlMode();

	// Set 2 second timer for when it ends
	GetWorldTimerManager().SetTimer(ControlTimerHandle, this, &AVerticleSliderPlatform::ResetPlayersControlMode, ControlTime, false);
}

void AVerticleSliderPlatform::ResetPlayersControlMode()
{
	if (PlayerRef == nullptr) return;
	PlayerRef->SetControlMode(false);
	WasHit = false;
	CanMove = false;
	OutControlMode();
	GetWorldTimerManager().ClearTimer(ControlTimerHandle);
}

void AVerticleSliderPlatform::Move(float DeltaTime)
{
	FVector StartLocation = GetActorLocation();

	SetActorRelativeLocation(FMath::Lerp(StartLocation, EndLocation, Speed * DeltaTime));
}
