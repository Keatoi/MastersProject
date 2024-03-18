// Masters Project - Owen S Atkinson


#include "TankGameMode.h"
#include "TankController.h"
#include "TankAIController.h"

ATankGameMode::ATankGameMode()
{
	PlayerControllerClass = ATankAIController::StaticClass();
}

void ATankGameMode::GameOverCheck()
{
}
