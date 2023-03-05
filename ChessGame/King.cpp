// Fill out your copyright notice in the Description page of Project Settings.


#include "King.h"


AKing::AKing()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hasMoved = false;
}



// Called when the game starts or when spawned
void  AKing::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AKing::GetHasMoved() const {
	return hasMoved;
}

void AKing::SetHasMoved(bool value) {
	this->hasMoved = value;
}