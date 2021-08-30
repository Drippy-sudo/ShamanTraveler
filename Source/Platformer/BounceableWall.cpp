// Fill out your copyright notice in the Description page of Project Settings.

#include "BounceableWall.h"
#include "Components/BoxComponent.h"
#include "PlayerPaperCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABounceableWall::ABounceableWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	RootComponent = RootComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABounceableWall::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABounceableWall::OnOverlapBegin);
	//PlayerRef = Cast<APlayerPaperCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ABounceableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABounceableWall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OtherActor = Cast<APlayerPaperCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//if (OtherActor != nullptr && PlayerRef != nullptr)
	//{
	//	PlayerRef->IsTouchingFront = true;
	//}
}
