// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ChessPlayer.h"
#include "ChessSquare.h"
#include "ChessPawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Move.h"
#include <vector>
#include "GameFramework/GameModeBase.h"
#include "ChessGameGameModeBase.generated.h"



class AMyPlayerController;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	AChessGameGameModeBase();
	virtual void BeginPlay() override;

public:
	void ShowLegalMoves();
	FVector WorldToBoardCoordinateTransform(FVector worldPosition);
	void highlightMoveList(std::list<AMove>& ml);
	void UnHighlightMoveList(std::list<AMove>& ml);
	void findBishopMoves(int row, int col);
	void findQueenMoves(int row, int col);
	void findRookMoves(int row, int col);
	void findKnightMoves(int row, int col);
	void findKingMoves(int row, int col);
	void findPawnMoves(int row, int col);
	void checkMoveDirection(int row, int col, int dirX, int dirY, int radius = 8);
	void checkSquareAt(int row, int col, int rowEnd, int colEnd);
	bool isKingInCheck(int row, int col, AMove discovery = AMove(FVector(-1,-1,0), FVector(-1, -1, 0), MoveType::EMPTY, -99));
	bool KingDirectionalCheck(int row, int col, int dirX, int dirY, bool isDiagonal, AMove discovery = AMove(FVector(-1, -1, 0), FVector(-1, -1, 0), MoveType::EMPTY, -99));
	int getPieceTypeAt(int row, int col, FVector& posOut);
	bool HandleCheckedSquare();
	void findAllLegalMoves();
	void Promote();
	void moveChessPiece(AChessSquare* chessSquare);
	void updateBoardState(FVector start, FVector end, MoveType moveType);
	void checkCastling();
	void printBoardState();
	void removeDiscoveryCheckMoves();
	void nextTurn();
	void highlightSelectedPiece(AChessPiece* chessPiece);
	void DeselectChessPiece(AChessPiece* chessPiece);
	AChessPiece* GetChessPieceSelection() const;
	void SetChessPieceSelection(AChessPiece* chessPiece, bool& selected);
	bool canTake(AChessPiece* pieceToTake,AChessSquare*& destinationSquare);
	UFUNCTION(BlueprintImplementableEvent)
	void ShowPawnPromotionWidget();


	UFUNCTION(BlueprintImplementableEvent)
	void ShowPawnPromotionWidgetB();

	UFUNCTION(BlueprintImplementableEvent)
	void WaitPawnPromotionInput();

	UFUNCTION(BlueprintCallable)
	void PromotePawn();

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AChessSquare> BP_chessSquare;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AChessPawn> BP_chessPawnW;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AChessPawn> BP_chessPawnB;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<ABishop> BP_BishopW;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<ABishop> BP_BishopB;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<ARook> BP_RookB;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<ARook> BP_RookW;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AKnight> BP_KnightW;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AKnight> BP_KnightB;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AQueen> BP_QueenB;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AQueen> BP_QueenW;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AKing> BP_KingW;

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	TSubclassOf<AKing> BP_KingB;

	AChessPiece* chessPieceArray[8][8];
	AChessSquare* squareArray[8][8];

	//initial board state
	int chessBoard[8][8] = {
		{-4, -2, -3, -5, -6, -3, -2,-4},
		{-1, -1, -1, -1, -1, -1, -1, -1},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{4, 2, 3, 5, 6, 3, 2, 4}
	};
	std::list<AMove> legalMoveList;
	bool waiting;
	int swapKing;
	AKing* kingW;
	AKing* kingB;
	AChessPlayer* whitePlayer;
	AChessPlayer* blackPlayer;
	AMyPlayerController* controller;
	AChessSquare* checkedSquare;
	AChessPiece* chessPieceSelection;
	AChessSquare* chessSquareSelection;
	AMove* previousMove; // for enpasant checking
	std::vector<AMove> moveHistory;
	std::vector<AChessPiece*> attackedPieces;
	PlayerType currentTurn = PlayerType::WHITE;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool check;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool checkMate;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool draw;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	int pawnPromoSelection;


};
