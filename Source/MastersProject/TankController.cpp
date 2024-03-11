// Masters Project - Owen S Atkinson


#include "TankController.h"

ATankController::ATankController()
{
	auto* PC = Cast<AChaosTankPawn>(GetPawn());
}

void ATankController::Tick(float DeltaTime)
{
}

void ATankController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	//Get Player Controller and Enhanced Input Subsystem

	
	//Bind Actions to functions
	UEnhancedInputComponent* Ei = Cast<UEnhancedInputComponent>(InputComponent);
	Ei->BindAction(InputMove, ETriggerEvent::Triggered, this, &ATankController::MoveTriggered);
	Ei->BindAction(InputMove, ETriggerEvent::Completed, this, &ATankController::MoveCompleted);
	Ei->BindAction(InputMove, ETriggerEvent::Canceled, this, &ATankController::MoveCancelled);
	Ei->BindAction(InputMove, ETriggerEvent::Started, this, &ATankController::MoveStarted);
	Ei->BindAction(InputTurn, ETriggerEvent::Triggered, this, &ATankController::TurnTriggered);
	Ei->BindAction(InputTurn, ETriggerEvent::Started, this, &ATankController::TurnStarted);
	Ei->BindAction(InputTurn, ETriggerEvent::Canceled, this, &ATankController::TurnCancelled);
	Ei->BindAction(InputTurn, ETriggerEvent::Completed, this, &ATankController::TurnCompleted);
	Ei->BindAction(InputLook, ETriggerEvent::Triggered, this, &ATankController::Look);
	Ei->BindAction(InputFirePrimary, ETriggerEvent::Started, this, &ATankController::PrimaryFire);
	Ei->BindAction(InputFireSecondary, ETriggerEvent::Started, this, &ATankController::SecondaryFireStart);
	Ei->BindAction(InputReloadIM, ETriggerEvent::Started, this, &ATankController::ReloadInteriorMagazine);
	Ei->BindAction(InputCameraSwap, ETriggerEvent::Started, this, &ATankController::CameraSwap);
	Ei->BindAction(InputDefaultCam, ETriggerEvent::Started, this, &ATankController::DefaultView);
	Ei->BindAction(InputZoomCam, ETriggerEvent::Started, this, &ATankController::CommanderView);
	Ei->BindAction(InputGunnerCam, ETriggerEvent::Started, this, &ATankController::GunnerView);
}

void ATankController::MoveTriggered(const FInputActionValue& Value)
{
}

void ATankController::MoveCompleted(const FInputActionValue& Value)
{
}

void ATankController::MoveStarted(const FInputActionValue& Value)
{
}

void ATankController::MoveCancelled(const FInputActionValue& Value)
{
}

void ATankController::TurnTriggered(const FInputActionValue& Value)
{
}

void ATankController::TurnStarted(const FInputActionValue& Value)
{
}

void ATankController::TurnCancelled(const FInputActionValue& Value)
{
}

void ATankController::TurnCompleted(const FInputActionValue& Value)
{
}

void ATankController::Look(const FInputActionValue& Value)
{
}

void ATankController::PrimaryFire(const FInputActionValue& Value)
{
}

void ATankController::SecondaryFireStart(const FInputActionValue& Value)
{
}

void ATankController::SecondaryFire()
{
}

void ATankController::SecondaryFireReleased(const FInputActionValue& Value)
{
}

void ATankController::CameraSwap(const FInputActionValue& Value)
{
}

void ATankController::DefaultView(const FInputActionValue& Value)
{
}

void ATankController::CommanderView(const FInputActionValue& Value)
{
}

void ATankController::GunnerView(const FInputActionValue& Value)
{
}

void ATankController::ReloadMG()
{
}

void ATankController::ReloadInteriorMagazine(const FInputActionValue& Value)
{
}
