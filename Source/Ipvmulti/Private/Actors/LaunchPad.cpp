// Fill out your copyright notice in the Description page of Project Settings.


#include "Ipvmulti/Public/Actors/LaunchPad.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"


// Sets default values
ALaunchPad::ALaunchPad()
{
	// Inicializacion de componentes
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	OverlapComp = CreateDefaultSubobject<UBoxComponent>("OverlapComp");
	// Se establece el overlap component como el root
	RootComponent = OverlapComp;
	// El mesh comp se hace hijo del overlap component (root)
	MeshComp->SetupAttachment(OverlapComp);
	LaunchForce = 1000;
	LaunchAngle = 90;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OverlapLaunchPad);
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchForce;
	ACharacter* MyChar = Cast<ACharacter>(OtherActor);

	if (MyChar)
	{
		MyChar -> LaunchCharacter(LaunchVelocity, true, true);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap"));
		}
	}
}

