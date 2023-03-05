// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "Knight.generated.h"

UCLASS()
class CHESSGAME_API AKnight : public AChessPiece
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKnight();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};