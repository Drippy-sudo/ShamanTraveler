// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftableBlock.h"

/*
* A interactable block that when hit you can move it up only and it stops the player moving.
* If moved to a certain height will move slowly downwards, so setting the time dialation to move slower.
* Else it will just drop back down to the ground like a physics object and kill enemies.
*/

void ALiftableBlock::BeginPlay()
{
	Super::BeginPlay();

	
}

void ALiftableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
