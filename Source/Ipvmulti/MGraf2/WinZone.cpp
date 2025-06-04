// Fill out your copyright notice in the Description page of Project Settings.


#include "WinZone.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AWinZone::AWinZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	BoxCollision->InitBoxExtent(FVector(50,50,50));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = BoxCollision;

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWinZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWinZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* hit = Cast<APlayerCharacter>(OtherActor))
	{
		hit->WinNotification();
	}
}