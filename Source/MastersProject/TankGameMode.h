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
	UFUNCTION()
	void GameOverCheck();
	UFUNCTION()
	void HandleGameOver();
protected:
	UPROPERTY()
	class ATankGameState* GS;
};
