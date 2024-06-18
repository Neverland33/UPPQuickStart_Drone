// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../UPPQuickStart_DroneGameModeBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;
	BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapHandler);

	BulletMovment = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovment->InitialSpeed = 2000.0f;
	BulletMovment->MaxSpeed = 2500.0f;

	this->InitialLifeSpan = 5.0f;

}

// 重叠事件时调用函数
void ABullet::OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		OtherActor->Destroy();
		this->Destroy();

		if (AUPPQuickStart_DroneGameModeBase* GameMode = Cast<AUPPQuickStart_DroneGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->Score++;
		}
	}

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

