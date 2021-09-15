// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "BaseInteractableBlock.generated.h"

class APlayerPaperCharacter;

UCLASS()
class PLATFORMER_API ABaseInteractableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractableBlock();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UPaperFlipbookComponent* PaperFlipbookComponent;
};
