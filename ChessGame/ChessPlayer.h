// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rook.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "ChessPlayer.generated.h"

enum PlayerType { WHITE = 1, BLACK = -1 };

UCLASS()
class CHESSGAME_API AChessPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChessPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UCameraComponent* PlayerCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ARook* shortRook;
	ARook* longRook;
	PlayerType playerType;
};
