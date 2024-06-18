// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// �����
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	// ������ײ���
	OutBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutBoxCollision"));
	OutBoxCollision->SetupAttachment(RootComponent);
	OutBoxCollision->SetSimulatePhysics(true);
	OutBoxCollision->SetBoxExtent(FVector(90.0f, 90.0f, 40.0f));
	OutBoxCollision->BodyInstance.bLockXRotation = true;
	OutBoxCollision->BodyInstance.bLockYRotation = true;
	OutBoxCollision->SetCollisionProfileName(TEXT("Pawn"));
	OutBoxCollision->SetMassOverrideInKg();
	OutBoxCollision->SetLinearDamping(2.0f);
	OutBoxCollision->SetAngularDamping(0.5f);

	// �����壺����ģ�ͼ��ĸ�����������
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OutBoxCollision);

	Paddle_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle_1"));
	Paddle_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle_2"));
	Paddle_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle_3"));
	Paddle_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle_4"));
	Paddle_1->SetupAttachment(Mesh, TEXT("Paddle_1"));
	Paddle_2->SetupAttachment(Mesh, TEXT("Paddle_2"));
	Paddle_3->SetupAttachment(Mesh, TEXT("Paddle_3"));
	Paddle_4->SetupAttachment(Mesh, TEXT("Paddle_4"));
	Paddles.Add(Paddle_1);
	Paddles.Add(Paddle_2);
	Paddles.Add(Paddle_3);
	Paddles.Add(Paddle_4);

	// ���������ƽ�����������Ϻ���ǰ
	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	UpThruster->SetupAttachment(RootComponent);
	UpThruster->SetAutoActivate(true);															// �Զ�����
	UpThruster->ThrustStrength = 980.0f;														// �ƽ�����С
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));	// ���÷���Ϊ����

	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->ThrustStrength = 0;
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));

	// ������͵��ɱ����
	DefaultSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("DefaultSpringArm"));
	DefaultSpringArm->SetupAttachment(RootComponent);
	DefaultSpringArm->TargetArmLength = 400.0f;
	DefaultSpringArm->bUsePawnControlRotation = true;

	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultCamera"));
	DefaultCamera->SetupAttachment(DefaultSpringArm);
	DefaultCamera->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();

	// �����ǿ������ϵͳ
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputSubsystem->AddMappingContext(DefaultIMC, 0);
		}
	}
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PaddleRotate(DeltaTime);

	// ǰ����б�ָ�
	if (!bHasForwardInput)
	{
		if (Mesh->GetRelativeRotation().Pitch != 0)
		{
			float curPitch = Mesh->GetRelativeRotation().Pitch;
			Mesh->AddRelativeRotation(FRotator(-curPitch * DeltaTime * 2.0f, 0.0f, 0.0f));
			if (FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
			{
				Mesh->SetRelativeRotation(FRotator(.0f, .0f, .0f));
			}
		}
	}

}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);

		EnhancedInputComponent->BindAction(LiftAction, ETriggerEvent::Triggered, this, &ADrone::Lift);
		EnhancedInputComponent->BindAction(LiftAction, ETriggerEvent::Completed, this, &ADrone::LiftFinish);

		EnhancedInputComponent->BindAction(ForwardAction, ETriggerEvent::Triggered, this, &ADrone::Forward);
		EnhancedInputComponent->BindAction(ForwardAction, ETriggerEvent::Completed, this, &ADrone::ForwardFinish);

		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ADrone::Turn);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ADrone::Fire);
	}

}

// �ƶ��ӽ�����
void ADrone::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

// �������봥��
void ADrone::Lift(const FInputActionValue& Value)
{
	float LiftFloatValue = Value.Get<float>();
	UpThruster->ThrustStrength += LiftFloatValue * LiftAcc * GetWorld()->DeltaTimeSeconds;
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength, -LiftThrustStrengthMax, LiftThrustStrengthMax);

}

// ����������������Բ�д������
void ADrone::LiftFinish(const FInputActionValue& Value)
{
	UpThruster->ThrustStrength = 980.0f;

}

// ǰ�����봥��
void ADrone::Forward(const FInputActionValue& Value)
{
	bHasForwardInput = true;

	float ForwardFloatValue = Value.Get<float>();
	ForwardThruster->ThrustStrength += ForwardFloatValue * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength, -ForwardThrustStrengthMax, ForwardThrustStrengthMax);

	float PitchValue = -ForwardFloatValue * Mesh->GetRelativeRotation().Pitch;
	if (PitchValue < 30.0f)
	{
		Mesh->AddRelativeRotation(FRotator(-ForwardFloatValue * ForwardLeanSpeed * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f));
	}

}

// ǰ���������
void ADrone::ForwardFinish(const FInputActionValue& Value)
{
	ForwardThruster->ThrustStrength = 0;

	bHasForwardInput = false;

}

// ��ת���봥��
void ADrone::Turn(const FInputActionValue& Value)
{
	float TurnFloatValue = Value.Get<float>();
	OutBoxCollision->AddTorqueInDegrees(this->GetActorUpVector() * TurnFloatValue * TurnStrength);

}

// �����ӵ�
void ADrone::Fire(const FInputActionValue& Value)
{
	FTransform FirePoint = Mesh->GetSocketTransform(TEXT("FireBullet_Socket"));
	GetWorld()->SpawnActor<ABullet>(BulletType, FirePoint);

	UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);

}

// ������ת��ÿ֡����
void ADrone::PaddleRotate(float DeltaTime)
{
	for (auto paddle : Paddles)
	{
		paddle->AddRelativeRotation(FRotator(0.0f, DeltaTime * PaddleRotateSpeed, 0.0f));
	}

}

