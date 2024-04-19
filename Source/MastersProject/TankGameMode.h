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
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	UFUNCTION()
	void GameOverCheck();
	UFUNCTION()
	void HandleGameOver();
protected:
	UPROPERTY()
	class ATankStateBase* GS;
};
