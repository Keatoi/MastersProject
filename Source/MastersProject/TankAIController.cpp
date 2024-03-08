// Masters Project - Owen S Atkinson


#include "TankAIController.h"

#include "AI_ChaosTank.h"

ATankAIController::ATankAIController(FObjectInitializer const& ObjectInitializer)
{
}

void ATankAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(AAI_ChaosTank* const npc = Cast<AAI_ChaosTank>(InPawn))
	{
		if(UBehaviorTree* const tree = npc->GetBehaviourTree())
		{
			UBlackboardComponent* Bb;
			UseBlackboard(tree->BlackboardAsset,Bb);
			Blackboard = Bb;
			RunBehaviorTree(tree);
		}
	}
}
