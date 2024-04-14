// Masters Project - Owen S Atkinson
//Had to remake GameState as a GameStateBase since for some reason GameState doesn't work with GameModeBase derived classes
//Sometimes Epic Games infuriates me to a level never before seen

#include "TankStateBase.h"

#include "Net/UnrealNetwork.h"

void ATankStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATankStateBase, RedTickets);
	DOREPLIFETIME(ATankStateBase, BlueTickets);
	DOREPLIFETIME(ATankStateBase, NeutralTickets);
	DOREPLIFETIME(ATankStateBase, GameEnum);
}

void ATankStateBase::DecreaseRed(float amount)
{
	RedTickets -= amount;
}

void ATankStateBase::IncreaseRed(float amount)
{
	RedTickets += amount;
}

void ATankStateBase::DecreaseBlue(float amount)
{
	BlueTickets -= amount;
}

void ATankStateBase::IncreaseBlue(float amount)
{
	BlueTickets += amount;
}
