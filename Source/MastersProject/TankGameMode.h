// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API ATankGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATankGameMode();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int BlueTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RedTickets = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int NeutralTickets = 0.f;
	UFUNCTION()
	void GameOverCheck();
};
