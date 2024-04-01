// Masters Project - Owen S Atkinson


#include "BTTask_CheckLOS.h"
#include "AI_ChaosTank.h"
#include "TankAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckLOS::UBTTask_CheckLOS(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check LOS";
}

EBTNodeResult::Type UBTTask_CheckLOS::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ATankAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Npc = Controller->GetPawn())
		{
			
			if (Npc->IsA<AAI_ChaosTank>())
			{
				//Movement Code
				//Cast To AITank
				AAI_ChaosTank* tank = Cast<AAI_ChaosTank>(Npc);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool("bHasLOS",tank->bEnemySpotted);
			
			}
			//Finish successfully
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
			

		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Type();
}
