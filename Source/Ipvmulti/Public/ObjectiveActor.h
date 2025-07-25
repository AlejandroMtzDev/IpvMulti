// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveActor.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class IPVMULTI_API AObjectiveActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObjectiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* meshComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* sphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* particleVFX;

	void PlayEffects();
	
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
