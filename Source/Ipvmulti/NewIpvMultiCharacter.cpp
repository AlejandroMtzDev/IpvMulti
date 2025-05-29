// Fill out your copyright notice in the Description page of Project Settings.


#include "NewIpvMultiCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Ipvmulti/Public/Actors/IpvMultiProjectile.h"

// Sets default values
ANewIpvMultiCharacter::ANewIpvMultiCharacter()
{
	// Set size for collision capsule
			GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	 
			// set our turn rate for input
			TurnRateGamepad = 50.f;
	 
			// Don't rotate when the controller rotates. Let that just affect the camera.
			bUseControllerRotationPitch = false;
			bUseControllerRotationYaw = false;
			bUseControllerRotationRoll = false;
	 
			// Configure character movement
			GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	 
			// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
			// instead of recompiling to adjust them
			GetCharacterMovement()->JumpZVelocity = 700.f;
			GetCharacterMovement()->AirControl = 0.35f;
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
			GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
			GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	 
			// Create a camera boom (pulls in towards the player if there is a collision)
			CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
			CameraBoom->SetupAttachment(RootComponent);
			CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
			CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	 
			// Create a follow camera
			FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
			FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
			FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	 
			// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
			// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	 
			//Initialize the player's Health
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

void ANewIpvMultiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////////////////////////////////////////
///// MOVIMIENTO

void ANewIpvMultiCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANewIpvMultiCharacter::Move);
	
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANewIpvMultiCharacter::Look);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ANewIpvMultiCharacter::StartFire);
	}
	
	// Set up gameplay key bindings
	// check(PlayerInputComponent);
	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	// PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//  
	// PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ANewIpvMultiCharacter::MoveForward);
	// PlayerInputComponent->BindAxis("Move Right / Left", this, &ANewIpvMultiCharacter::MoveRight);
	//  
	// // We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// // "turn" handles devices that provide an absolute delta, such as a mouse.
	// // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	// PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	// PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ANewIpvMultiCharacter::TurnAtRate);
	// PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	// PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ANewIpvMultiCharacter::LookUpAtRate);
	//  
	// // handle touch devices
	// PlayerInputComponent->BindTouch(IE_Pressed, this, &ANewIpvMultiCharacter::TouchStarted);
	// PlayerInputComponent->BindTouch(IE_Released, this, &ANewIpvMultiCharacter::TouchStopped);
	//  
	// // Handle firing projectiles
	// PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANewIpvMultiCharacter::StartFire);
}

void ANewIpvMultiCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ANewIpvMultiCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/////////////////////////////////////////////////////////////
///// REPLICACION

// Called every frame
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


