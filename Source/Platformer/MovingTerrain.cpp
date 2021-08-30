// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingTerrain.h"

AMovingTerrain::AMovingTerrain()
{
    // Default parameters...
    // Can tick
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);

    Speed = 20.0f;

    CanMove = false;
}

void AMovingTerrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CanMove)
    {
        // Moves cube to widget target
        // Get actor location
        FVector Location = GetActorLocation();

        float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
        float JourneyTravelled = (Location - GlobalStartLocation).Size();

        // Swaps target and start around to go back and forth
        if (JourneyTravelled > JourneyLength)
        {
            //FVector Swap = GlobalStartLocation;
            //GlobalStartLocation = GlobalTargetLocation;
            //GlobalTargetLocation = Swap;

            Speed = 0;
            CanMove = false;
            ReachedDestination();
        }

        FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();

        // Increases magnitude in a direction
        Location += Speed * DeltaTime * Direction;

        // Set actor as this location
        SetActorLocation(Location);
    }
}

void AMovingTerrain::BeginPlay()
{
    Super::BeginPlay();

    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}