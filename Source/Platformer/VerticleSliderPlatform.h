// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VerticleSliderPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API AVerticleSliderPlatform : public AActor
{
	GENERATED_BODY()
	
public:
	AVerticleSliderPlatform();
	
public:
	void Move(float DeltaTime);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Can it move
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove;

	// Distance it will move
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;

	// Speed at which it will travel
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasHit;

	UPROPERTY(EditDefaultsOnly)
	float ControlTime;

	void ResetPlayersControlMode();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	FTimerHandle ControlTimerHandle;

	class APlayerPaperCharacter* PlayerRef;

	FVector EndLocation;
};
