// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UI/MainMenu.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI_API AMenuGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UMainMenu* levelUI;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainMenu> gameUI_Class;
};
