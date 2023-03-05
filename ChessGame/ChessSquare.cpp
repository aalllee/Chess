// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessSquare.h"

// Sets default values
AChessSquare::AChessSquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	chessSquareMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("chess piece mesh"));
	chessSquareMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AChessSquare::BeginPlay()
{
	Material = chessSquareMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	chessSquareMesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("emission"), 0.0f);
	Super::BeginPlay();
}
// Called every frame
void AChessSquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AChessSquare::setMaterialEmission(const float& emission, const FVector& color) {
	DynamicMaterial->SetScalarParameterValue(TEXT("emission"), emission);
	DynamicMaterial->SetVectorParameterValue(TEXT("emissiveColor"), color);
}

