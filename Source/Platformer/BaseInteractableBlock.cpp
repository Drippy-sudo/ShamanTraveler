// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseInteractableBlock.h"

// Sets default values
ABaseInteractableBlock::ABaseInteractableBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	RootComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	StaticMeshComp->SetupAttachment(RootComp);

	PaperFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Paper Flipbook Comp"));
	PaperFlipbookComponent->SetupAttachment(StaticMeshComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->SetupAttachment(PaperFlipbookComponent);
}
