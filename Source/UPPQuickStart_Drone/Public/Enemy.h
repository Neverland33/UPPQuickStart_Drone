// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class UPPQUICKSTART_DRONE_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	// Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* EnemyMesh;

	// ��ը���Ӻ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosionEffect")
	class UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExplosionEffect")
	class USoundBase* ExplosionSound;

	// ���������ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating")
	float FloatSpeed = 250.0f;

	// ����ʱ���ú���
	UFUNCTION()
	void DestroyHandler(AActor* DestroyedActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
