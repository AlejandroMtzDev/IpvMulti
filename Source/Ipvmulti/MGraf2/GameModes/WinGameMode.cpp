// Fill out your copyright notice in the Description page of Project Settings.


#include "WinGameMode.h"

void AWinGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (gameUI_Class)
	{
		levelUI = CreateWidget<UWinScreenMenu>(GetWorld(), gameUI_Class);

		levelUI->AddToViewport();
	}
}
