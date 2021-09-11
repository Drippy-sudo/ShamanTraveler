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

public:
	ALiftableBlock();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void InControlMode();

	UFUNCTION(BlueprintImplementableEvent)
	void OutControlMode();

	void ResetPlayersControlMode();

	void Move(float DeltaTime);

	void SetSlowFallMode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ControlTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Distance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SlowFallTimeDialation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsInSlowFallMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector TargetLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector BeginLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector StartFallLocation;

	APlayerPaperCharacter* PlayerRef;

	bool bIsTargeted;

	FTimerHandle ControlTimerHandle;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector DesiredLocation;
};
