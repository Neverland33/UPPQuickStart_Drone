// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
	EnemyMesh->SetGenerateOverlapEvents(true);

	this->OnDestroyed.AddDynamic(this, &AEnemy::DestroyHandler);

	this->InitialLifeSpan = 10.0f;

}

// 销毁时调用函数
void AEnemy::DestroyHandler(AActor* DestroyedActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, this->GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->AddActorWorldOffset(FVector(0.0f, 0.0f, 1.0f) * FloatSpeed * DeltaTime);

}

