// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "Bullet.h"
#include "Drone.generated.h"

UCLASS()
class UPPQUICKSTART_DRONE_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

	// 组件
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OutBoxCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle_1;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle_2;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle_3;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle_4;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* UpThruster;
	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* ForwardThruster;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* DefaultSpringArm;
	UPROPERTY(visibleAnywhere)
	class UCameraComponent* DefaultCamera;

	// 输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultIMC;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LiftAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ForwardAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* TurnAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	// 移动控制参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementDefaults")
	float LiftAcc = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementDefaults")
	float LiftThrustStrengthMax = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementDefaults")
	float ForwardAcc = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementDefaults")
	float ForwardThrustStrengthMax = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementDefaults")
	float ForwardLeanSpeed = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementDefaults")
	float TurnStrength = 500000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaddleDefaults")
	float PaddleRotateSpeed = 2000.0f;

	// 子弹设置
	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<ABullet> BulletType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Bullet")
	USoundBase* BulletSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 输入绑定函数
	void Look(const FInputActionValue& Value);
	void Lift(const FInputActionValue& Value);
	void LiftFinish(const FInputActionValue& Value);
	void Forward(const FInputActionValue& Value);
	void ForwardFinish(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

	// 旋翼旋转，每帧调用
	void PaddleRotate(float DeltaTime);
	TArray<UStaticMeshComponent*> Paddles;

	bool bHasForwardInput = false;

};
