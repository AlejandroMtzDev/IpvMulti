// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IpvMultiPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI_API AIpvMultiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionCompleted(APawn* InstigatorPawn, bool bIsMissionSuccess);
};
