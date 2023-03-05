// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "ChessGameGameModeBase.h"
AMyPlayerController::AMyPlayerController() {

	bShowMouseCursor = 1;
	bEnableClickEvents = 1;
	bEnableMouseOverEvents = 1;
	chessPieceSelection = nullptr;
	gameModePointer = nullptr;
}

void AMyPlayerController::BeginPlay() {
	//cache gamemode pointer
	gameModePointer = dynamic_cast<AChessGameGameModeBase*>(GetWorld()->GetAuthGameMode());
}

void AMyPlayerController::OnLeftMouseClick()
{	
	if (gameModePointer->waiting) {return;}
	bool selected = false;
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, true,hit);
	AActor* actorPointer = hit.GetActor();
	if (dynamic_cast<AChessPiece*>(actorPointer) != nullptr) {
		gameModePointer->SetChessPieceSelection(dynamic_cast<AChessPiece*>(actorPointer), selected);
		if(selected)
			gameModePointer->ShowLegalMoves();
	}
	else if (dynamic_cast<AChessSquare*>(actorPointer) != nullptr) {
		// ptr is object of ChessSquare class
		if (gameModePointer->GetChessPieceSelection() == nullptr) {
			return;
		}
		AChessSquare* destinationSquare = dynamic_cast<AChessSquare*>(actorPointer);
		gameModePointer->moveChessPiece(destinationSquare);
		return;
	}
	if (dynamic_cast<AChessPiece*>(actorPointer) != nullptr && gameModePointer->GetChessPieceSelection() != nullptr) {
		AChessSquare* destinationSquare = nullptr;
		if (gameModePointer->canTake(dynamic_cast<AChessPiece*>(actorPointer), destinationSquare)) {
			gameModePointer->moveChessPiece(destinationSquare);
		}
	}
}




void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Bind actions for mouse events
	InputComponent->BindAction("LMB", IE_Pressed, this, &AMyPlayerController::OnLeftMouseClick);
}





