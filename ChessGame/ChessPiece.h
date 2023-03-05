// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "ChessPiece.generated.h"

class UCurveFloat;

enum pieceType {
	WHITE_PAWN=1,
	BLACK_PAWN = -1,

	WHITE_KNIGHT=2,
	BLACK_KNIGHT = -2,

	WHITE_BISHOP=3,
	BLACK_BISHOP=-3,

	WHITE_ROOK=4,
	BLACK_ROOK=-4,

	WHITE_QUEEN=5,
	BLACK_QUEEN=-5,

	WHITE_KING=6,
	BLACK_KING=-6,
};

UCLASS()
class CHESSGAME_API AChessPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();

	UFUNCTION()
	void TimelineProgress(float value);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TimelineFinishedCallback();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* chessPieceMesh;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;
	FTimeline CurveTimeline;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* getMeshPointer() const;
	void setMaterialEmission(const float& emission, const FVector& color = FVector(1, 0.525, 0.0));
	void setType(pieceType pt);
	pieceType getType() const;
	void move(FVector squareLocation);

private:
	UMaterialInterface* Material;
	UMaterialInstanceDynamic* DynamicMaterial;
	pieceType type;

	//move animation variables
	FVector startPosition;
	FVector destinationSquare;
};
