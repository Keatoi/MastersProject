// Masters Project - Owen S Atkinson



#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TestPlayerCamera = CreateDefaultSubobject<UCameraComponent>("Cam");
	TestPlayerCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Get Player Controller and Enhanced Input Subsystem
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		EI->BindAction(InputMove,ETriggerEvent::Triggered,this,&APlayerCharacter::Move);
		EI->BindAction(InputLook,ETriggerEvent::Triggered,this,&APlayerCharacter::Look);
		EI->BindAction(InputLook,ETriggerEvent::Triggered,this,&APlayerCharacter::PrimaryFire);
		
	}
	

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//movement code
	UE_LOG(LogTemp, Display, TEXT("move value: %f"), Value.Get<float>());
	const FVector2d MoveValue = Value.Get<FVector2d>();
	const FRotator MoveRot(0,Controller->GetControlRotation().Yaw,0);
	if(MoveValue.Y != 0.0f)//Forward/Backwards
	{
		const FVector Direction = MoveRot.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction,MoveValue.Y);
	}
	if(MoveValue.X != 0.0f)
	{
		const FVector Direction = MoveRot.RotateVector(FVector::RightVector);
		AddMovementInput(Direction, MoveValue.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	if (LookValue.X != 0.f)
	{
		AddControllerYawInput(LookValue.X);
	}
 
	if (LookValue.Y != 0.f)
	{
		AddControllerPitchInput(LookValue.Y);
	}
}

void APlayerCharacter::PrimaryFire(const FInputActionValue& Value)
{
	FHitResult Hit;
	FVector TraceStart = GetActorLocation();
	//Use a range dependant on projectile later, just use 10,000 cm for now
	FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * 10000.0f;
	FCollisionQueryParams QueryParams;
	//add player to ignored actors to prevent self-harm
	QueryParams.AddIgnoredActor(this);
	//Use LineTracebySingle for now, may need to change later if projectiles are too fast
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
		
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
	}
	
 
	
	
	
	
}

