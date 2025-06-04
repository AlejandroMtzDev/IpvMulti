// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyObject.h"

#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AKeyObject::AKeyObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(15.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = SphereComponent;

	SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnImpact);
	
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
void AKeyObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKeyObject::OnImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
	{
		Destroy();
	}
}

void AKeyObject::Pick()
{
	Destroy();
}