// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessSquare.generated.h"

UCLASS()
class CHESSGAME_API AChessSquare : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessSquare();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* chessSquareMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void setMaterialEmission(const float& emission, const FVector& color = FVector(0.942708, 0.352616,0.0));

private:
	UMaterialInterface* Material;
	UMaterialInstanceDynamic* DynamicMaterial;
};
