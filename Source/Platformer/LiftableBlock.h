// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractableBlock.h"
#include "LiftableBlock.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API ALiftableBlock : public ABaseInteractableBlock
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
