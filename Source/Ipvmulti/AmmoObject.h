// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoObject.generated.h"

UCLASS()
class IPVMULTI_API AAmmoObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmoObject();

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	void Pick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
