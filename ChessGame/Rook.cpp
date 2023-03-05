// Fill out your copyright notice in the Description page of Project Settings.


#include "Rook.h"


ARook::ARook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hasMoved = false;
}
// Called when the game starts or when spawned
void  ARook::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void ARook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
bool ARook::GetHasMoved() const {
	return hasMoved;
}
void ARook::SetHasMoved(bool value) {
	this->hasMoved = value;
}