// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "King.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AKing : public AChessPiece
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AKing();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool GetHasMoved() const;
	void SetHasMoved(bool value);

private:
	bool hasMoved;
};
