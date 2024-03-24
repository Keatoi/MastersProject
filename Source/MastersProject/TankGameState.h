// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TankGameState.generated.h"

UENUM(BlueprintType)
enum EGameState
{
	ESTARTED UMETA(DisplayName = "Game Started/Ongoing"),
	EREDWIN UMETA(DisplayName = "Red Victory"),
	EBLUEWIN UMETA(DisplayName = "Blue Victory")
};
UCLASS()
class MASTERSPROJECT_API ATankGameState : public AGameState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int BlueTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RedTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int NeutralTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<EGameState> GameEnum;
	
};
