// Fill out your copyright notice in the Description page of Project Settings.


#include "Move.h"


AMove::AMove()
{
 	
}

AMove::AMove(FVector start, FVector end, MoveType mt, int pt)
{
	moveStart=start;
	moveEnd=end;
	moveType=mt;
	pieceType=pt;
}

AMove::~AMove()
{


}




