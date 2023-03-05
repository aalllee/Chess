// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChessGameGameModeBase.h"
#include "MyPlayerController.h"
AChessGameGameModeBase::AChessGameGameModeBase() {

//	DefaultPawnClass = AChessPlayer::StaticClass();
	
}
void AChessGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	moveHistory = {};
	legalMoveList = {};
	attackedPieces = {};
	chessPieceSelection = nullptr;
	checkedSquare = nullptr;
	check = checkMate = draw = waiting = false;
	swapKing = 1;
	pawnPromoSelection = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			chessPieceArray[i][j] = nullptr;
		}
	}

	//cache player pawns for switching
	whitePlayer = nullptr;
	blackPlayer = nullptr;
	TSubclassOf<AChessPlayer> classToFind = AChessPlayer::StaticClass();
	TArray<AActor*> foundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, foundPlayers);
	for (int i = 0; i < foundPlayers.Num(); i++) {
		if (foundPlayers[i]) {
			FString name = foundPlayers[i]->GetActorNameOrLabel();
			if (name.Equals(FString("BP_ChessPlayerW")))
				whitePlayer = dynamic_cast<AChessPlayer*>(foundPlayers[i]);
			if (name.Equals(FString("BP_ChessPlayerB")))
				blackPlayer = dynamic_cast<AChessPlayer*>(foundPlayers[i]);
		}
	}
	if (whitePlayer) { whitePlayer->playerType = PlayerType::WHITE; }
	if (blackPlayer) { blackPlayer->playerType = PlayerType::BLACK; }


	
	//Spawn Chess Squares
	if (BP_chessSquare) {

		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				AChessSquare* CSp = GetWorld()->SpawnActor<AChessSquare>(BP_chessSquare, FVector(-350.0f + 100 * i, 350.0f - 100 * j, 5.0f), FRotator(0, 0, 0));
				squareArray[7 - j][0 + i] = CSp;
			}
		}
	}
	//Spawn Pawns
	if (BP_chessPawnW) {
		for (int i = 0; i < 8; ++i) {
			FVector PawnPosition = FVector(-350.0f + 100 * i, 350.0f - 100 * 1, 5.0f);
			 AChessPawn* ChessPawnp = GetWorld()->SpawnActor<AChessPawn>(BP_chessPawnW, PawnPosition, FRotator(0, 0, 0));
			 if (ChessPawnp) {
				 ChessPawnp->setType(WHITE_PAWN);
				 chessPieceArray[6][i] = ChessPawnp;
			 }
			
		}
	}
	if (BP_chessPawnB) {
		for (int i = 0; i < 8; ++i) {
			FVector PawnPosition = FVector(-350.0f + 100 * i, 350.0f - 100 * 6, 5.0f);
			AChessPawn* ChessPawnp = GetWorld()->SpawnActor<AChessPawn>(BP_chessPawnB, PawnPosition, FRotator(0, 0, 0));
			ChessPawnp->setType(BLACK_PAWN);
			chessPieceArray[1][i] = ChessPawnp;
		}
	}
	//Spawn_Bishops
	if (BP_BishopW) {
			//a3 bishop
			FVector BishopPosition = FVector(-350.0f + 100 * 2, 350.0f , 5.0f);
			ABishop* Bishop_p = GetWorld()->SpawnActor<ABishop>(BP_BishopW, BishopPosition, FRotator(0, 0, 0));
			Bishop_p->setType(WHITE_BISHOP);
			chessPieceArray[7][2] = Bishop_p;

			// a6 bishop
			BishopPosition = FVector(-350.0f + 100 * 5, 350.0f, 5.0f);
			Bishop_p = GetWorld()->SpawnActor<ABishop>(BP_BishopW, BishopPosition, FRotator(0, 0, 0));
			Bishop_p->setType(WHITE_BISHOP);
			chessPieceArray[7][5] = Bishop_p;
	}
	if (BP_BishopB) {
		//H3 bishop
		FVector BishopPosition = FVector(-350.0f + 100 * 2, 350.0f - 100 * 7, 5.0f);
		ABishop* Bishop_p = GetWorld()->SpawnActor<ABishop>(BP_BishopB, BishopPosition, FRotator(0, 0, 0));
		Bishop_p->setType(BLACK_BISHOP);
		chessPieceArray[0][2] = Bishop_p;

		// H6 bishop
		BishopPosition = FVector(-350.0f + 100 * 5, 350.0f - 100 * 7, 5.0f);
		Bishop_p = GetWorld()->SpawnActor<ABishop>(BP_BishopB, BishopPosition, FRotator(0, 0, 0));
		Bishop_p->setType(BLACK_BISHOP);
		chessPieceArray[0][5] = Bishop_p;
	}
	//Spawn_Rooks
	if (BP_RookW) {
		//a1 rook
		FVector rookPosition = FVector(-350.0f, 350.0f, 5.0f);
		ARook* rook_p = GetWorld()->SpawnActor<ARook>(BP_RookW, rookPosition, FRotator(0, 0, 0));
		rook_p->setType(WHITE_ROOK);
		whitePlayer->longRook = rook_p;
		chessPieceArray[7][0] = rook_p;

		// a8 rook
		rookPosition = FVector(350.0f, 350.0f, 5.0f);
		rook_p = GetWorld()->SpawnActor<ARook>(BP_RookW, rookPosition, FRotator(0, 0, 0));
		rook_p->setType(WHITE_ROOK);
		whitePlayer->shortRook = rook_p;
		chessPieceArray[7][7] = rook_p;
	}
	if (BP_RookB) {
		//a8 rook
		FVector rookPosition = FVector(-350.0f, -350.0f, 5.0f);
		ARook* rook_p = GetWorld()->SpawnActor<ARook>(BP_RookB, rookPosition, FRotator(0, 0, 0));
		rook_p->setType(BLACK_ROOK);
		blackPlayer->longRook = rook_p;
		chessPieceArray[0][0] = rook_p;

		// h8 rook
		rookPosition = FVector(350.0f, -350.0f, 5.0f);
		rook_p = GetWorld()->SpawnActor<ARook>(BP_RookB, rookPosition, FRotator(0, 0, 0));
		rook_p->setType(BLACK_ROOK);
		blackPlayer->shortRook = rook_p;
		chessPieceArray[0][7] = rook_p;
	}
	//Spawn_Knights
	if (BP_KnightW) {
		//b1 knight
		FVector knightPosition = FVector(-250.0f, 350.0f, 5.0f);
		AKnight* knight_p = GetWorld()->SpawnActor<AKnight>(BP_KnightW, knightPosition, FRotator(0, 0, 0));
		knight_p->setType(WHITE_KNIGHT);
		chessPieceArray[7][1] = knight_p;

		// g1 knight
		knightPosition = FVector(250.0f, 350.0f, 5.0f);
		knight_p = GetWorld()->SpawnActor<AKnight>(BP_KnightW, knightPosition, FRotator(0, 0, 0));
		knight_p->setType(WHITE_KNIGHT);
		chessPieceArray[7][6] = knight_p;
	}
	if (BP_KnightB) {
		//b8 knight
		FVector knightPosition = FVector(-250.0f, -350.0f, 5.0f);
		AKnight* knight_p = GetWorld()->SpawnActor<AKnight>(BP_KnightB, knightPosition, FRotator(0, 0, 0));
		knight_p->setType(BLACK_KNIGHT);
		chessPieceArray[0][1] = knight_p;

		// g8 knight
		knightPosition = FVector(250.0f, -350.0f, 5.0f);
		knight_p = GetWorld()->SpawnActor<AKnight>(BP_KnightB, knightPosition, FRotator(0, 0, 0));
		knight_p->setType(BLACK_KNIGHT);
		chessPieceArray[0][6] = knight_p;
	}
	//Spawn Queens
	if (BP_QueenW) {
		//d1 queen
		FVector QueenPosition = FVector(-50.0f, 350.0f, 5.0f);
		AQueen* queen_p = GetWorld()->SpawnActor<AQueen>(BP_QueenW, QueenPosition, FRotator(0, 0, 0));
		queen_p->setType(WHITE_QUEEN);
		chessPieceArray[7][3] = queen_p;
	}
	if (BP_QueenB) {
		//d8 queen
		FVector QueenPosition = FVector(-50.0f, -350.0f, 5.0f);
		AQueen* queen_p = GetWorld()->SpawnActor<AQueen>(BP_QueenB, QueenPosition, FRotator(0, 0, 0));
		queen_p->setType(BLACK_QUEEN);
		chessPieceArray[0][3] = queen_p;
	}
	//Spawn Kings
	if (BP_KingW) {
		//d1 king
		FVector KingPosition = FVector(50.0f, 350.0f, 5.0f);
		AKing* king_p = GetWorld()->SpawnActor<AKing>(BP_KingW, KingPosition, FRotator(0, 0, 0));
		king_p->setType(WHITE_KING);
		chessPieceArray[7][4] = king_p;
		kingW = king_p;
	}
	if (BP_KingB) {
		//d8 king
		FVector KingPosition = FVector(50.0f, -350.0f, 5.0f);
		AKing* king_p = GetWorld()->SpawnActor<AKing>(BP_KingB, KingPosition, FRotator(0, 0, 0));
		king_p->setType(BLACK_KING);
		chessPieceArray[0][4] = king_p;
		kingB = king_p;
	}
	
	//controller init
	if (GetWorld()->GetFirstPlayerController())
		controller = dynamic_cast<AMyPlayerController*>(GetWorld()->GetFirstPlayerController());
	
	if(controller)
		controller->Possess(whitePlayer);

}

//move search functions
void AChessGameGameModeBase::ShowLegalMoves() {
	//shows legal moves for current selection
	UnHighlightMoveList(legalMoveList);

	if (chessPieceSelection == nullptr || checkMate) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("Select a chess piece first"));
		return;
	}

	FVector boardVector = WorldToBoardCoordinateTransform(chessPieceSelection->GetActorLocation());
	int row = FMath::RoundToInt(boardVector.X);
	int col = FMath::RoundToInt(boardVector.Y);
	int chessPieceType = chessBoard[row][col];

	switch (chessPieceType) {
	case 1:
	case -1:
		findPawnMoves(row, col);
		break;

	case 2:
	case -2:
		findKnightMoves(row, col);
		break;

	case 3:
	case -3:
		findBishopMoves(row, col);
		break;

	case 4:
	case -4:
		findRookMoves(row, col);
		break;

	case 5:
	case -5:
		findQueenMoves(row, col);
		break;

	case 6:
	case -6:
		findKingMoves(row, col);
		break;

	default:
		//debug
		UE_LOG(LogTemp, Display, TEXT("no case detected: %s"), *FString(("piece type:" + FString::FromInt(chessPieceType))));
		UE_LOG(LogTemp, Display, TEXT("world coords: %s"), *FString(chessPieceSelection->GetActorLocation().ToString()));
		UE_LOG(LogTemp, Display, TEXT("BOARD VECTOR: %s"), *FString(boardVector.ToString()));
		UE_LOG(LogTemp, Display, TEXT("board coords: %s"), *FString(FString::FromInt(row) + FString::FromInt(col)));
	}

	removeDiscoveryCheckMoves();
	highlightMoveList(legalMoveList);
}
void AChessGameGameModeBase::findPawnMoves(int row, int col) {

	int startingRow, lastRow;
	(currentTurn * swapKing == WHITE) ? startingRow = 6 : startingRow = 1;
	(currentTurn * swapKing == WHITE) ? lastRow = 0 : lastRow = 7;
	MoveType moveT;
			
	int lCol = col - 1;
	int rCol = col + 1;
	int pieceType = chessBoard[row][col];
	int frontRow = row - 1 * currentTurn*swapKing;

	//check if a pawn can take on its left diagonal
	if ((0 <= frontRow && frontRow <= 7) && ((0 <= lCol) && (lCol <= 7))) {
		if (currentTurn * swapKing * chessBoard[frontRow][lCol] < 0) {
			(frontRow == lastRow) ? moveT = PROMOTION : moveT = TAKES;
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(frontRow, lCol, 0), moveT, pieceType));
		}
	}

	//check if a pawn can take on its right diagonal
	if ((0 <= frontRow && frontRow <= 7) && ((0 <= rCol) && (rCol <= 7))) {
		if (currentTurn * swapKing * chessBoard[frontRow][rCol] < 0) {
			(frontRow == lastRow) ? moveT = PROMOTION : moveT = TAKES;
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(frontRow, rCol, 0), moveT, pieceType));
		}
	}

	//check if a pawn can take on its right diagonal via ENPASANT
	if ((0 <= frontRow && frontRow <= 7) && ((0 <= rCol) && (rCol <= 7)) && !moveHistory.empty()) {
		if ((chessBoard[row][rCol]*chessBoard[row][col] == -1 ) && moveHistory.back().moveEnd == FVector(row, rCol, 0) && moveHistory.back().moveType == DOUBLE_PAWN)
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(frontRow, rCol, 0), ENPASANT, pieceType));
	}

	//check if a pawn can take on its left diagonal via ENPASANT
	if ((0 <= frontRow && frontRow <= 7) && ((0 <= lCol) && (lCol <= 7)) && !moveHistory.empty()) {
		if ((chessBoard[row][lCol] * chessBoard[row][col] == -1) && moveHistory.back().moveEnd == FVector(row, lCol, 0) && moveHistory.back().moveType == DOUBLE_PAWN)
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(frontRow, lCol, 0), ENPASANT, pieceType));
	}
		
	//check if a pawn can go straght by 1 square
	if ((0 <= frontRow && frontRow <= 7) && ((0 <= col) && (col <= 7))) {
		if (chessBoard[frontRow][col] == 0) {
			(frontRow == lastRow) ? moveT = PROMOTION : moveT = EMPTY;
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(frontRow, col, 0), moveT, pieceType));
		}
		else {
			return;
		}
	}

	//check if a pawn can go straght by 2 squares
	frontRow = row - 2 * currentTurn * swapKing;
	if (row == startingRow) {
		if (chessBoard[frontRow][col] == 0) {
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(frontRow, col, 0), DOUBLE_PAWN, pieceType));
		}
	}
}
void AChessGameGameModeBase::findBishopMoves(int row, int col) {	
	checkMoveDirection(row, col, 1, 1);
	checkMoveDirection(row, col, 1, -1);
	checkMoveDirection(row, col, -1, 1);
	checkMoveDirection(row, col, -1, -1);
}
void AChessGameGameModeBase::findQueenMoves(int row, int col) {
	findBishopMoves(row, col);
	findRookMoves(row, col);
}
void AChessGameGameModeBase::findKingMoves(int row, int col) {

	if(!isKingInCheck(row,col))
		checkCastling();

	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0)
				continue;
			checkMoveDirection(row, col, i, j, 2);
		}
	}
}
void AChessGameGameModeBase::findRookMoves(int row, int col) {
	checkMoveDirection(row, col, 1, 0);
	checkMoveDirection(row, col, -1, 0);
	checkMoveDirection(row, col, 0, 1);
	checkMoveDirection(row, col, 0, -1);
}
void AChessGameGameModeBase::findKnightMoves(int row, int col) {
	checkSquareAt(row, col, row - 2, col + 1);
	checkSquareAt(row, col, row - 2, col - 1);
	checkSquareAt(row, col, row - 1, col + 2);
	checkSquareAt(row, col, row - 1, col - 2);
	checkSquareAt(row, col, row + 1, col - 2);
	checkSquareAt(row, col, row + 1, col + 2);
	checkSquareAt(row, col, row + 2, col + 1);
	checkSquareAt(row, col, row + 2, col - 1);
}
bool AChessGameGameModeBase::HandleCheckedSquare() {
	//check if enemy king is in check after current move
	//also check if mate or draw after each move

	AKing* king_p = nullptr;
	swapKing = -1;
	(currentTurn == WHITE) ? king_p = kingB : king_p = kingW;
	FVector kingPos = WorldToBoardCoordinateTransform(king_p->GetActorLocation());
	
	if (checkedSquare)
		checkedSquare->setMaterialEmission(0.0f);


	checkedSquare = nullptr;
	check = false;
	checkMate = false;
	draw = false;

	
	if (isKingInCheck(kingPos.X, kingPos.Y)) {
		check = true;
		checkedSquare = squareArray[int(kingPos.X)][int(kingPos.Y)];
		checkedSquare->setMaterialEmission(20.0f, FVector(1, 0, 0));

		//if no legal moves found and king in check -> check mate
		findAllLegalMoves();
		if (legalMoveList.empty()) {
			checkMate = true;
		}
		else {
			//debug
		}
			
	}
	else {
		findAllLegalMoves();
		if (legalMoveList.empty()) {
			//if no legal moves but not in check -> draw
			draw = true;
		}
	}
	swapKing = 1;
	return true;
}
void AChessGameGameModeBase::findAllLegalMoves() {
	legalMoveList.clear();
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (chessBoard[i][j] * swapKing * currentTurn > 0) {
				switch (chessBoard[i][j]) {
				case 1:
				case -1:
					findPawnMoves(i, j);
					break;
				case 2:
				case -2:
					findKnightMoves(i, j);
					break;
				case 3:
				case -3:
					findBishopMoves(i, j);
					break;
				case 4:
				case -4:
					findRookMoves(i, j);
					break;
				case 5:
				case -5:
					findQueenMoves(i, j);
					break;
				case 6:
				case -6:
					findKingMoves(i, j);
					break;
				}
			}
		}
	}
	removeDiscoveryCheckMoves();
}
void AChessGameGameModeBase::Promote()
{
	//handle mesh swap for pawn promotion

	AChessPiece* promotedPiece = nullptr;
	AMove pawnMove = moveHistory.back();
	AChessPiece* pawn = chessPieceSelection;
	FVector worldPos = chessPieceSelection->GetActorLocation();
	pawn->Destroy();

	switch (chessBoard[int(pawnMove.moveEnd.X)][int(pawnMove.moveEnd.Y)]) {
	case 2:
		promotedPiece = GetWorld()->SpawnActor<AKnight>(BP_KnightW, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(WHITE_KNIGHT);
		break;

	case -2:
		promotedPiece = GetWorld()->SpawnActor<AKnight>(BP_KnightB, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(BLACK_KNIGHT);
		break;

	case 3:
		promotedPiece = GetWorld()->SpawnActor<ABishop>(BP_BishopW, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(WHITE_BISHOP);
		break;

	case -3:
		promotedPiece = GetWorld()->SpawnActor<ABishop>(BP_BishopB, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(BLACK_BISHOP);
		break;

	case 4:
		promotedPiece = GetWorld()->SpawnActor<ARook>(BP_RookW, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(WHITE_ROOK);
		break;

	case -4:
		promotedPiece = GetWorld()->SpawnActor<ARook>(BP_RookB, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(BLACK_ROOK);
		break;

	case 5:
		promotedPiece = GetWorld()->SpawnActor<AQueen>(BP_QueenW, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(WHITE_QUEEN);
		break;

	case -5:
		promotedPiece = GetWorld()->SpawnActor<AQueen>(BP_QueenB, worldPos, FRotator(0, 0, 0));
		promotedPiece->setType(BLACK_QUEEN);
		break;
	}
	chessPieceArray[int(pawnMove.moveEnd.X)][int(pawnMove.moveEnd.Y)] = promotedPiece;
	pawnPromoSelection = 0;
}
void AChessGameGameModeBase::PromotePawn() {
	//function called after player selects promotion type
	pawnPromoSelection *= currentTurn;
	moveChessPiece(chessSquareSelection);
}
void AChessGameGameModeBase::checkCastling() {
	bool shortCastle = true;
	bool longCastle = true;
	int kingsRow, swap;
	AKing* king_p = nullptr;
	AChessPlayer* player_p;

	if (currentTurn * swapKing == WHITE) {
		kingsRow = 7;
		swap = 1;
		king_p = kingW;
		player_p = whitePlayer;
	}
	else
	{
		kingsRow = 0;
		swap = -1;
		king_p = kingB;
		player_p = blackPlayer;
	}

	if (king_p->GetHasMoved()) {
		return;
	}

	//check short side blocks
	if (chessBoard[kingsRow][5] != 0 || chessBoard[kingsRow][6] != 0)
		shortCastle = false;

	//check long side blocks
	if (chessBoard[kingsRow][2] != 0 || chessBoard[kingsRow][1] != 0)
		longCastle = false;

	//check if short rook exists and if it has been moved
	if (player_p->shortRook == nullptr || player_p->shortRook->GetHasMoved())
		shortCastle = false;

	//check if long rook exists and if it has been moved
	if (player_p->longRook == nullptr || player_p->longRook->GetHasMoved())
		longCastle = false;

	//check if the king would be in check along the short castling path
	if (isKingInCheck(kingsRow, 5) || isKingInCheck(kingsRow, 6))
		shortCastle = false;

	//check if the king would be in check along the long castling path
	if (isKingInCheck(kingsRow, 3) || isKingInCheck(kingsRow, 2) || isKingInCheck(kingsRow, 1))
		longCastle = false;

	if (shortCastle) {
		legalMoveList.push_back(AMove(FVector(kingsRow, 4, 0), FVector(kingsRow, 6, 0), CASTLE_SHORT, 6 * swap));
	}
	if (longCastle) {
		legalMoveList.push_back(AMove(FVector(kingsRow, 4, 0), FVector(kingsRow, 2, 0), CASTLE_LONG, 6 * swap));
	}
}
void AChessGameGameModeBase::removeDiscoveryCheckMoves() {
	FVector kingPos;
	if (currentTurn * swapKing == WHITE && kingW)
		kingPos = WorldToBoardCoordinateTransform(kingW->GetActorLocation());
	else if (currentTurn * swapKing == BLACK && kingB)
		kingPos = WorldToBoardCoordinateTransform(kingB->GetActorLocation());

	int row = kingPos.X;
	int col = kingPos.Y;
	std::vector<AMove> toRemove = {};
	for (const auto& move : legalMoveList)
	{
		if (isKingInCheck(row, col, move)) {
			toRemove.push_back(move);
		}
	}
	for (int i = 0; i < toRemove.size(); ++i) {
		legalMoveList.remove(toRemove[i]);
	}
}

//utility functions
void AChessGameGameModeBase::printBoardState() {
	//used for debug
	FString MyString = "";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			MyString += FString::FromInt(chessBoard[i][j]);

		}
		UE_LOG(LogTemp, Display, TEXT("Printing string: %s"), *MyString);
		MyString = "";
	}


	MyString = "";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			FString str;
			if (!chessPieceArray[i][j]) {
				str = "1";
			}
			else {
				str = "0";
			}

			MyString += str;

		}
		UE_LOG(LogTemp, Display, TEXT("Printing string: %s"), *MyString);
		MyString = "";
	}


}
FVector AChessGameGameModeBase::WorldToBoardCoordinateTransform(FVector worldPosition) {

	FVector arrayPosition;
	//X coordinate -350 -> 350 :: on board a->h :: array column 0->7
	arrayPosition.Y = FMath::RoundToInt((worldPosition.X + 350.0f) / 100.0f);
	
	//Y coordinate 350 -> -350 :: on board 1->8 :: array row 0->7
	arrayPosition.X = FMath::RoundToInt((worldPosition.Y + 350.0f) / 100.0f);

	arrayPosition.Z = 0;

	return arrayPosition;
}
void AChessGameGameModeBase::nextTurn() {
	HandleCheckedSquare();
	if (currentTurn == WHITE) {
		controller->Possess(blackPlayer);
		currentTurn = PlayerType::BLACK;
	}
	else {
		controller->Possess(whitePlayer);
		currentTurn = PlayerType::WHITE;
	}
	DeselectChessPiece(chessPieceSelection);
	waiting = false;
}
bool AChessGameGameModeBase::canTake(AChessPiece* pieceToTake, AChessSquare*& destinationSquare)
{
	FVector piecePos = WorldToBoardCoordinateTransform(pieceToTake->GetActorLocation());
	for (auto& move : legalMoveList) {
		if (move.moveType == TAKES && piecePos.X == move.moveEnd.X && piecePos.Y == move.moveEnd.Y) {
			destinationSquare = squareArray[(int)piecePos.X][(int)piecePos.Y];
			return true;
		}
	}
	return false;
}
AChessPiece* AChessGameGameModeBase::GetChessPieceSelection() const {
	return chessPieceSelection;
}
void AChessGameGameModeBase::updateBoardState(FVector start, FVector end, MoveType moveType) {
	//function parameters should be in board coordinates not World

	int row = start.X;
	int col = start.Y;
	int pieceType = chessBoard[row][col];
	AChessPiece* chessPiece_p = chessPieceArray[row][col];
	AChessPiece* toDestroy = nullptr;
	chessPieceArray[row][col] = nullptr;
	chessBoard[row][col] = 0;
	row = end.X;
	col = end.Y;
	if (moveType == MoveType::ENPASANT) {row = start.X;}

	//if piece gets captured during a move destroy the actor
	if (chessBoard[row][col] != 0) {
		if (chessPieceArray[row][col] != nullptr) {
			toDestroy = chessPieceArray[row][col];
			toDestroy->Destroy();
			chessPieceArray[row][col] = nullptr;
			if (moveType == MoveType::ENPASANT) {
				chessBoard[row][col] = 0;
			}
		}
	}

	row = end.X;
	col = end.Y;
	chessBoard[row][col] = pieceType;
	chessPieceArray[row][col] = chessPiece_p;

	if (moveType == MoveType::PROMOTION) {
		if (chessPieceArray[row][col] != nullptr) {
			toDestroy = chessPieceArray[row][col];
			chessBoard[row][col] = pawnPromoSelection;
		}
	}
}
void AChessGameGameModeBase::checkSquareAt(int row, int col, int rowEnd, int colEnd) {
	MoveType moveType;
	int endSquareState = currentTurn * swapKing * chessBoard[rowEnd][colEnd];
	if ((0 <= rowEnd && rowEnd <= 7) && ((0 <= colEnd) && (colEnd <= 7))) {
		if (endSquareState <= 0) {
			endSquareState == 0 ? moveType = EMPTY : moveType = TAKES;
			legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(rowEnd, colEnd, 0), moveType, chessBoard[row][col]));
		}
	}
}
bool AChessGameGameModeBase::isKingInCheck(int row, int col, AMove discovery) {
	bool kingChecked = false;
	bool isDiagonal;
	int numberOfThreats = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0)
				continue;
			isDiagonal = bool(FMath::Abs(i * j));
			kingChecked = kingChecked || KingDirectionalCheck(row, col, i, j, isDiagonal, discovery);
		}
	}

	if (kingChecked)
		++numberOfThreats;

	if (discovery.pieceType == -6 || discovery.pieceType == 6) {
		row = discovery.moveEnd.X;
		col = discovery.moveEnd.Y;
	}

	//Check for surrounding knights
	int enemyKnight = -2 * currentTurn * swapKing;
	FVector posOut;
	if (getPieceTypeAt(row - 2, col + 1, posOut) == enemyKnight
		|| getPieceTypeAt(row - 1, col + 2, posOut) == enemyKnight
		|| getPieceTypeAt(row + 1, col + 2, posOut) == enemyKnight
		|| getPieceTypeAt(row + 2, col + 1, posOut) == enemyKnight
		|| getPieceTypeAt(row + 2, col - 1, posOut) == enemyKnight
		|| getPieceTypeAt(row + 1, col - 2, posOut) == enemyKnight
		|| getPieceTypeAt(row - 1, col - 2, posOut) == enemyKnight
		|| getPieceTypeAt(row - 2, col - 1, posOut) == enemyKnight) {

		kingChecked = true;

		if (discovery.moveType == TAKES && discovery.moveEnd.X == posOut.X && discovery.moveEnd.Y == posOut.Y && numberOfThreats==0)
			kingChecked = false;
	}

	return kingChecked;
}
bool AChessGameGameModeBase::KingDirectionalCheck(int row, int col, int dirX, int dirY, bool isDiagonal, AMove discovery) {

	if (discovery.pieceType == 6 || discovery.pieceType == -6) {
		//no discovery check can be made if the piece moved is a king
		row = discovery.moveEnd.X;
		col = discovery.moveEnd.Y;
	}

	for (int i = 1; i < 8; i++) {
		int newRow = row + i * dirY;
		int newCol = col + i * dirX;
		int pieceType = chessBoard[newRow][newCol];

		if ((0 <= newRow && newRow <= 7) && (0 <= newCol && newCol <= 7)) {

			if (discovery.moveType == ENPASANT && newRow == discovery.moveEnd.X + (1 * currentTurn * swapKing) && newCol == discovery.moveEnd.Y) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("no threat if enpasant"));
				pieceType = 0;
			}

			if (discovery.moveStart.X == newRow && discovery.moveStart.Y == newCol) {
				//starting square of a discovery move is empty
				pieceType = 0;
			}

			if (discovery.moveEnd.X == newRow && discovery.moveEnd.Y == newCol) {
				//new position of friendly piece
				pieceType = 99;
			}

			if (pieceType == -5 * currentTurn * swapKing
				|| (pieceType == -3 * currentTurn * swapKing && isDiagonal)
				|| (pieceType == -6 * currentTurn * swapKing && i == 1)
				|| (pieceType == -4 * currentTurn * swapKing && !isDiagonal)
				|| (pieceType == -1 * currentTurn * swapKing && i == 1 && isDiagonal && (swapKing * currentTurn * (row - newRow) > 0))) {

				return true;
				break;
			}
			else if (pieceType == 0) {
				continue;
			}
			else {
				break;
			}
		}
	}
	return false;
}
int AChessGameGameModeBase::getPieceTypeAt(int row, int col, FVector& posOut) {
	posOut = FVector(row, col, 0);
	if ((0 <= row && row <= 7) && (0 <= col && col <= 7)) {
		return chessBoard[row][col];
	}
	else
	{
		return -99;
	}

}
void AChessGameGameModeBase::checkMoveDirection(int row, int col, int dirX, int dirY, int radius) {
	//checks for available squares to move to starting from {row,col} in a direction defined by {dirX,dirY} within given radius.
	int newRow = row;
	int newCol = col;

	for (int i = 1; i < radius; i++) {
		newRow = row + dirY * i;
		newCol = col + dirX * i;

		if ((0 <= newRow && newRow <= 7) && (0 <= newCol && newCol <= 7)) {
			if (chessBoard[newRow][newCol] * currentTurn * swapKing == 0) {
				legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(newRow, newCol, 0), EMPTY, chessBoard[row][col]));
			}
			else if (chessBoard[newRow][newCol] * currentTurn * swapKing < 0) {
				legalMoveList.push_back(AMove(FVector(row, col, 0), FVector(newRow, newCol, 0), TAKES, chessBoard[row][col]));
				break;
			}
			else
			{
				break;
			}
		}
	}
}
void AChessGameGameModeBase::moveChessPiece(AChessSquare* chessSquare) {

	if (chessPieceSelection == nullptr)
		return;

	AChessPlayer* player_p;
	FVector clickedBoardPosition = WorldToBoardCoordinateTransform(chessSquare->GetActorLocation());
	FVector chessPieceBoardPosition = WorldToBoardCoordinateTransform(chessPieceSelection->GetActorLocation());


	AMove chessMove;
	bool legalSquare = false;
	for (const auto& move : legalMoveList)
	{
		if (clickedBoardPosition == move.moveEnd) {
			legalSquare = true;
			chessMove = move;
			break;
		}
	}

	if (legalSquare) {

		int kingsRow;
		waiting = true;

		(chessMove.pieceType > 0) ? kingsRow = 7 : kingsRow = 0;
		(chessMove.pieceType > 0) ? player_p = whitePlayer : player_p = blackPlayer;

		if (chessMove.moveType == CASTLE_SHORT) {
			//moving a rook if castling registered
			chessPieceArray[kingsRow][7]->move(squareArray[kingsRow][5]->GetActorLocation());
			updateBoardState(FVector(kingsRow, 7, 0), FVector(kingsRow, 5, 0), chessMove.moveType);
			player_p->shortRook->SetHasMoved(true);
		}
		if (chessMove.moveType == CASTLE_LONG) {
			//moving a rook if castling registered
			chessPieceArray[kingsRow][0]->move(squareArray[kingsRow][3]->GetActorLocation());
			updateBoardState(FVector(kingsRow, 0, 0), FVector(kingsRow, 3, 0), chessMove.moveType);
			player_p->longRook->SetHasMoved(true);
		}

		if (chessMove.moveType == PROMOTION && pawnPromoSelection == 0) {
			chessSquareSelection = chessSquare;
			//display a promotion selection widget
			(currentTurn == WHITE) ? ShowPawnPromotionWidget() : ShowPawnPromotionWidgetB();
			dynamic_cast<AChessPawn*>(chessPieceSelection)->isPromoting = true;
			return;
		}

		//start chess piece move animation
		chessPieceSelection->move(chessSquare->GetActorLocation());

		//handle move data
		moveHistory.push_back(chessMove);
		updateBoardState(chessPieceBoardPosition, clickedBoardPosition, chessMove.moveType);

		//set castling states
		if (chessMove.pieceType == 6)
			kingW->SetHasMoved(true);

		if (chessMove.pieceType == -6)
			kingB->SetHasMoved(true);

		if (chessMove.pieceType == 4)
			dynamic_cast<ARook*>(chessPieceSelection)->SetHasMoved(true);

		if (chessMove.pieceType == -4)
			dynamic_cast<ARook*>(chessPieceSelection)->SetHasMoved(true);


		//end turn
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AChessGameGameModeBase::nextTurn, 2.5f);
		UnHighlightMoveList(legalMoveList);
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("Select a legal move"));
	}

}

//visual
void AChessGameGameModeBase::highlightSelectedPiece(AChessPiece* chessPiece) {
	chessPiece->setMaterialEmission(20.0f);
}
void AChessGameGameModeBase::DeselectChessPiece(AChessPiece* chessPiece) {
	chessPiece->setMaterialEmission(0.0f);
	chessPieceSelection = nullptr;
}
void AChessGameGameModeBase::SetChessPieceSelection(AChessPiece* chessPiece, bool& selected) {

	//Allow only own piece selection
	if (currentTurn == WHITE && chessPiece->getType() < 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("can only select white pieces"));
		return;
	}
	if (currentTurn == BLACK && chessPiece->getType() > 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString("can only select black pieces"));
		return;
	}

	if (chessPieceSelection == nullptr) {
		chessPieceSelection = chessPiece;
	}	
	else {
		DeselectChessPiece(chessPieceSelection);
		chessPieceSelection = chessPiece;
	}

	highlightSelectedPiece(chessPieceSelection);
	selected = true;
}
void AChessGameGameModeBase::highlightMoveList(std::list<AMove>& ml) {

	int row, col;
	for (const auto& move : ml)
	{
		row = move.moveEnd.X;
		col = move.moveEnd.Y;
		squareArray[row][col]->setMaterialEmission(10.0f);

		if (move.moveType == TAKES) {
			chessPieceArray[row][col]->setMaterialEmission(10.0f, FVector(1, 0, 0));
			//attackedPieces.push_back(chessPieceArray[row][col]);
		}
			

	}
}
void  AChessGameGameModeBase::UnHighlightMoveList(std::list<AMove>& ml) {
	int row, col;
	for (const auto& move : ml)
	{
		row = move.moveEnd.X;
		col = move.moveEnd.Y;
		squareArray[row][col]->setMaterialEmission(0.0f);

		if (move.moveType == TAKES) {
			chessPieceArray[row][col]->setMaterialEmission(0.0f);
		}
	}
	legalMoveList.clear();
}