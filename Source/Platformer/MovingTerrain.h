// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingTerrain.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API AMovingTerrain : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingTerrain();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (MakeEditWidget = "true"))
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove;

	UFUNCTION(BlueprintImplementableEvent)
	void ReachedDestination();

protected:
	virtual void BeginPlay() override;

private:
	FVector GlobalStartLocation;
	FVector GlobalTargetLocation;
	
};
