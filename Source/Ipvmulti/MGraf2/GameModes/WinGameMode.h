// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Ipvmulti/MGraf2/UI/WinScreenMenu.h"
#include "WinGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI_API AWinGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UWinScreenMenu* levelUI;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWinScreenMenu> gameUI_Class;
};
