// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	//CreateWidget<UMainMenu>(GetWorld(), gameUI_Class);
	
	if (gameUI_Class)
	{
		levelUI = CreateWidget<UMainMenu>(GetWorld(), gameUI_Class);

		levelUI->AddToViewport();
	}
}

