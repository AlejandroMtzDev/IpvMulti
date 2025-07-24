// Fill out your copyright notice in the Description page of Project Settings.


#include "IpvMultiGameState.h"

#include "Player/IpvMultiPlayerController.h"

void AIpvMultiGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	// if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	// {
	// 	if (APawn* Pawn = PC->GetPawn())
	// 	{
	// 		Pawn->DisableInput(nullptr);
	// 	}
	// }

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AIpvMultiPlayerController* PC = Cast<AIpvMultiPlayerController>(It->Get());

		if (PC)
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			APawn* Pawn = PC->GetPawn();

			if (Pawn)
			{
				Pawn->DisableInput(nullptr);
			}
		}
	}
}
