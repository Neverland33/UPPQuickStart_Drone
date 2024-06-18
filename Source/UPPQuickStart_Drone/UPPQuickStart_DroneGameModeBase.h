// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UPPQuickStart_DroneGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UPPQUICKSTART_DRONE_API AUPPQuickStart_DroneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Score;
	
};
