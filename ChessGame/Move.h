// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum MoveType {
	CASTLE_SHORT = -2,
	CASTLE_LONG,
	EMPTY,
	TAKES,
	DOUBLE_PAWN,
	ENPASANT,
	PROMOTION,
	CHECK,
	CHECKMATE
};

class CHESSGAME_API AMove 
{

	
public:	
	// Sets default values for this actor's properties
	AMove();
	AMove(FVector start, FVector end, MoveType mt, int pt);
	~AMove();

	bool operator == (const AMove& m) const { return moveStart == m.moveStart && moveEnd == m.moveEnd; }
	MoveType moveType;
	FVector moveEnd;
	FVector moveStart;
	int pieceType;

};
