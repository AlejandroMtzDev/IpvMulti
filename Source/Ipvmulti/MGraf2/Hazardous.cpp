// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazardous.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
AHazardous::AHazardous()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	BoxCollision->InitBoxExtent(FVector(50,50,50));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = BoxCollision;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	DangerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DangerZone"));
	DangerZone->SetupAttachment(RootComponent);
	DangerZone->InitBoxExtent(FVector(60,60,60));
	DangerZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	DangerZone->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	
	if (CubeMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(CubeMesh.Object);
	}
}

// Called when the game starts or when spawned
void AHazardous::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHazardous::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHazardous::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* hit = Cast<APlayerCharacter>(OtherActor))
	{
		hit->GetDamage();
	}
}
