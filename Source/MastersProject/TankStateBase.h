// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TankStateBase.generated.h"

UENUM(BlueprintType)
enum EGameState
{
	//Dictates if game is in progress or if Red/Blue won
	ESTARTED UMETA(DisplayName = "Game Started/Ongoing"),
	EREDWIN UMETA(DisplayName = "Red Victory"),
	EBLUEWIN UMETA(DisplayName = "Blue Victory")
};
UCLASS()
class MASTERSPROJECT_API ATankStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	int BlueTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	int RedTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	int NeutralTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
	TEnumAsByte<EGameState> GameEnum;
};
