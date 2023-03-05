// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "ChessSquare.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"
class AChessGameGameModeBase;
/**
 * 
 */
UCLASS()
class CHESSGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	AMyPlayerController();

public:
	
	//virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void BeginPlay();
	void OnLeftMouseClick();

	AChessPiece* chessPieceSelection;
	AChessGameGameModeBase* gameModePointer;
};
