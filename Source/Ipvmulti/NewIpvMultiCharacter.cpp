// Fill out your copyright notice in the Description page of Project Settings.


#include "NewIpvMultiCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Ipvmulti/Public/Actors/IpvMultiProjectile.h"

// Sets default values
ANewIpvMultiCharacter::ANewIpvMultiCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//Initialize projectile class
	ProjectileClass = AIpvMultiProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

// Called when the game starts or when spawned
void ANewIpvMultiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANewIpvMultiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANewIpvMultiCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &ANewIpvMultiCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void ANewIpvMultiCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void ANewIpvMultiCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + ( GetActorRotation().Vector()  * 100.0f ) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();
	 
	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;
	 
	AIpvMultiProjectile* spawnedProjectile = GetWorld()->SpawnActor<AIpvMultiProjectile>(spawnLocation, spawnRotation, spawnParameters);
}

void ANewIpvMultiCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANewIpvMultiCharacter, CurrentHealth);
}

void ANewIpvMultiCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ANewIpvMultiCharacter::OnHealthUpdate()
{
	
	//Client-specific functionality
    if (IsLocallyControlled())
    {
    	FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
    	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
     
    	if (CurrentHealth <= 0)
    	{
    		FString deathMessage = FString::Printf(TEXT("You have been killed."));
    		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
    	}
    }
     
    //Server-specific functionality
    if (GetLocalRole() == ROLE_Authority)
    {
    	FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
    	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
    }
	
	//Functions that occur on all machines.
	/*
		Any special functionality that should occur as a result of damage or death should be placed here.
	*/
}

void ANewIpvMultiCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float ANewIpvMultiCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}


