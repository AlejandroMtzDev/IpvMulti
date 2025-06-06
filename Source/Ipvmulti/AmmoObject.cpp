// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoObject.h"

#include "NewIpvMultiCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AAmmoObject::AAmmoObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(15.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	
	//Definition for the Mesh that will serve as your visual representation.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	if (DefaultMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(DefaultMesh.Object);
	}
}

// Called when the game starts or when spawned
void AAmmoObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ANewIpvMultiCharacter* player = Cast<ANewIpvMultiCharacter>(OtherActor))
	{
		player->SetCurrentAmmo(5);
	}
}
