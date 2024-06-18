// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class UPPQUICKSTART_DRONE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	// 组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UProjectileMovementComponent* BulletMovment;

	// 重叠事件时调用函数
	UFUNCTION()
	void OverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
