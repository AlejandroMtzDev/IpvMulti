// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Ipvmulti/IpvmultiGameMode.h"
#include "Ipvmulti/NewIpvMultiCharacter.h"

// Sets default values
AObjectiveZone::AObjectiveZone()
{
	bReplicates = true;

	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Component"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = OverlapComponent;
	OverlapComponent->SetHiddenInGame(false);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComponent->DecalSize = FVector(1,1,1);
	DecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AObjectiveZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObjectiveZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleOverlap);
}

void AObjectiveZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10.0f,
			FColor::Green,
			FString::Printf(TEXT("Overlapped"))
		);
	}

	ANewIpvMultiCharacter* Pawn = Cast<ANewIpvMultiCharacter>(OtherActor);

	if (Pawn == nullptr) return;
	{
		if (Pawn->bIsCarryingObjective)
		{
			AIpvmultiGameMode* GameMode = Cast<AIpvmultiGameMode>(GetWorld()->GetAuthGameMode());

			if (GameMode)
			{
				GameMode->CompleteMission(Pawn);
			}
		}
	}
	
}

