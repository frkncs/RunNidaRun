// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunNidaRunCharacter.h"
#include "RunNidaRunProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine.h"


//////////////////////////////////////////////////////////////////////////
// ARunNidaRunCharacter

ARunNidaRunCharacter::ARunNidaRunCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ARunNidaRunCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	_defSpeed = GetMovementComponent()->GetMaxSpeed();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ARunNidaRunCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARunNidaRunCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARunNidaRunCharacter::StopSprint);
		
		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARunNidaRunCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ARunNidaRunCharacter::StopCrouch);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunNidaRunCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARunNidaRunCharacter::Look);
	}
}


void ARunNidaRunCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		MovementVector.Normalize();

		// add movement 
		ACharacter::AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		ACharacter::AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ARunNidaRunCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		ACharacter::AddControllerYawInput(LookAxisVector.X);
		ACharacter::AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARunNidaRunCharacter::StartSprint()
{
	auto characterMovement = GetCharacterMovement();
	characterMovement->MaxWalkSpeed = characterMovement->MaxCustomMovementSpeed;
}

void ARunNidaRunCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = _defSpeed;
}

void ARunNidaRunCharacter::StartCrouch()
{
	ACharacter::Crouch();
}

void ARunNidaRunCharacter::StopCrouch()
{
	ACharacter::UnCrouch();
}

void ARunNidaRunCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ARunNidaRunCharacter::GetHasRifle()
{
	return bHasRifle;
}