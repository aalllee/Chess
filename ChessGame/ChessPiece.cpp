// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	chessPieceMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("chess piece mesh"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	chessPieceMesh->SetupAttachment(RootComponent);
}

void AChessPiece::TimelineProgress(float value)
{
	FVector NewLocation = FMath::Lerp(startPosition, destinationSquare, value);
	SetActorLocation(NewLocation);
}

// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	Material = chessPieceMesh->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	chessPieceMesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("emission"), 0.0f);
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);
	}
}

void AChessPiece::TimelineFinishedCallback(){}

void AChessPiece::setType(pieceType pt) {type = pt;}
pieceType AChessPiece::getType() const { return type; }

// Called every frame
void AChessPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
}

void AChessPiece::setMaterialEmission(const float& emission, const FVector& color) {
	DynamicMaterial->SetScalarParameterValue(TEXT("emission"), emission);
	DynamicMaterial->SetVectorParameterValue(TEXT("color"), color);
}

UStaticMeshComponent* AChessPiece::getMeshPointer() const {
	return chessPieceMesh;
}

void AChessPiece::move(FVector squareLocation) {
	destinationSquare = squareLocation;
	startPosition = GetActorLocation();
	CurveTimeline.PlayFromStart();
}

