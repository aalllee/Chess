// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawn.h"
#include "ChessGameGameModeBase.h"
// Sets default values
AChessPawn::AChessPawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isPromoting = false;
}



// Called when the game starts or when spawned
void AChessPawn::BeginPlay()
{
	Super::BeginPlay();
	gameModePointer = dynamic_cast<AChessGameGameModeBase*>(GetWorld()->GetAuthGameMode());
	if (CurveFloat)
	{
		FOnTimelineEventStatic onTimelineFinishedCallbackPawn;
		onTimelineFinishedCallbackPawn.BindUFunction(this, FName{ TEXT("TimelineFinishedCallbackPawn") });
		CurveTimeline.SetTimelineFinishedFunc(onTimelineFinishedCallbackPawn);
	}
}

// Called every frame
void AChessPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessPawn::move() {
	
	

}

void AChessPawn::TimelineFinishedCallbackPawn() {
	if(isPromoting || gameModePointer->pawnPromoSelection != 0)
		gameModePointer->Promote();
}