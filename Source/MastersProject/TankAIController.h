// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API ATankAIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit ATankAIController(FObjectInitializer const& ObjectInitializer);
protected:
	virtual void OnPossess(APawn* InPawn) override;


	
	
};
