// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpwaner.generated.h"

UCLASS()
class UPPQUICKSTART_DRONE_API AEnemySpwaner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpwaner();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	TSubclassOf<AEnemy> EnemyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
	float SpawnTimeInterval = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnEnemy();

private:
	FTimerHandle SpawnTimerHandle;

};
