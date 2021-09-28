// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticSpinningBlade.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlayerPaperCharacter.h"

// Sets default values
AStaticSpinningBlade::AStaticSpinningBlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetupAttachment(MeshComp);

	Damage = 50.0f;
	KnockbackAmount = -200.0f;
}

// Called when the game starts or when spawned
void AStaticSpinningBlade::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AStaticSpinningBlade::OnOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AStaticSpinningBlade::OnOverlapEnd);
}

// Called every frame
void AStaticSpinningBlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaticSpinningBlade::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActive)
	{
		// Cast to player  
		APlayerPaperCharacter* Player = Cast<APlayerPaperCharacter>(OtherActor);
		if (Player)
		{
			// deal damage
			Player->IncomingDamageToApply(Damage);

			if (Player->PlayerMovementComp)
			{
				// Apply impulse to knock the player back
				FVector Direction;
				Direction = Player->GetActorForwardVector() * KnockbackAmount;

				//Player->PlayerMovementComp->AddImpulse(Direction, true);
				Player->LaunchCharacter(Direction, true, true);

				// play sound effect
				UGameplayStatics::PlaySound2D(GetWorld(), ImpactSound);

				IsActive = false;
			}
		}
	}
}

void AStaticSpinningBlade::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsActive = true;
}
