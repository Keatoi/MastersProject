// Masters Project - Owen S Atkinson


#include "TankGameState.h"

#include "Net/UnrealNetwork.h"
// This function is required through the replicated specifier in the UPROPERTY macro and is declared by it
void ATankGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATankGameState, RedTickets);
	DOREPLIFETIME(ATankGameState, BlueTickets);
	DOREPLIFETIME(ATankGameState, NeutralTickets);
	DOREPLIFETIME(ATankGameState, GameEnum);
}
