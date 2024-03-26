// Masters Project - Owen S Atkinson


#include "TankGameMode.h"
#include "TankController.h"
#include "TankAIController.h"

#include "TankStateBase.h"
#include "Kismet/GameplayStatics.h"

ATankGameMode::ATankGameMode()
{
	//Set Defaults
	PlayerControllerClass = ATankAIController::StaticClass();
	GameStateClass = ATankStateBase::StaticClass();
	GS = Cast<ATankStateBase>(UGameplayStatics::GetGameState(this));
}

void ATankGameMode::GameOverCheck()
{
	//Get Tickets and check for any winners
	if(GS->RedTickets <= 0.f)
	{
		//Red has no tickets left, assume Blue victory
		GS->GameEnum = EBLUEWIN;
		HandleGameOver();
	}
	else if(GS->BlueTickets <= 0.f)
	{
		//Blue has no tickets left, assume red victory
		GS->GameEnum  = EREDWIN;
		HandleGameOver();
	}
	else
	{
		//Game still in progress
		GS->GameEnum = ESTARTED;
		HandleGameOver();
	}
}

void ATankGameMode::HandleGameOver()
{
	//Open Game Over screen here
}
