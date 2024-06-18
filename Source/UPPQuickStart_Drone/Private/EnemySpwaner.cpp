// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpwaner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpwaner::AEnemySpwaner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));

}

// Called when the game starts or when spawned
void AEnemySpwaner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpwaner::SpawnEnemy, SpawnTimeInterval, true);
	
}

// Called every frame
void AEnemySpwaner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpwaner::SpawnEnemy()
{
	FVector Origin;
	FVector Extent;
	GetActorBounds(false, Origin, Extent);
	FVector SpawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	GetWorld()->SpawnActor<AEnemy>(EnemyType, SpawnPoint, FRotator::ZeroRotator);

}

