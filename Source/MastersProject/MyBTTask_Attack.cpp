// Masters Project - Owen S Atkinson


#include "MyBTTask_Attack.h"

#include "AI_ChaosTank.h"
#include "TankAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

class AAI_ChaosTank;

UMyBTTask_Attack::UMyBTTask_Attack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Attack";
}

EBTNodeResult::Type UMyBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(auto* const controller = Cast<ATankAIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* const Npc = controller->GetPawn())
		{
			//Cast to AI tank and call attack
			if (Npc->IsA<AAI_ChaosTank>())
			{
				//Movement Code
				//Cast To AITank
				AAI_ChaosTank* tank = Cast<AAI_ChaosTank>(Npc);
				FVector TargetLocation = tank->GetActorLocation();
				OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",TargetLocation);
				tank->Attack();
			
			}
			
		}
		return EBTNodeResult::Failed;
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
