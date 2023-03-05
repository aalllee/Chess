// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"

#include "ChessPawn.generated.h"

class AChessGameGameModeBase;

UCLASS()
class CHESSGAME_API AChessPawn : public AChessPiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPawn();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void move();

	UFUNCTION()
	void TimelineFinishedCallbackPawn();

	bool isPromoting;
	AChessGameGameModeBase* gameModePointer;
	
};
