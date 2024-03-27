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
	//Overide Team Attitude function so we can change how the AI responds to teams, could potentially be used for 1v1v1 matches if I get enough time
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	UFUNCTION()
	void OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus);
	UPROPERTY(VisibleAnywhere, Category = AI)
	//According to Epic this is how UE will handle pointers going forward. Raw pointers isn't going away anytime soon apparently but I thought I'd add them here for practice
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = nullptr;
	TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight = nullptr;
	TObjectPtr<class UAISenseConfig_Hearing> AISenseConfigHearing = nullptr;
	UPROPERTY(VisibleAnywhere, Category = AI)
	FGenericTeamId TeamID = FGenericTeamId(0);
protected:
	virtual void OnPossess(APawn* InPawn) override;


	
	
};
