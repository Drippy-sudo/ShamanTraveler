// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticSpinningBlade.generated.h"

UCLASS()
class PLATFORMER_API AStaticSpinningBlade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticSpinningBlade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
		class USphereComponent* SphereComp;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float Damage;

	// Must be negative
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float KnockbackAmount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool IsActive = true;
	
};
