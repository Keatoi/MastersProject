// Masters Project - Owen S Atkinson


#include "BTTask_InitTargetLoc.h"

#include "TankAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_InitTargetLoc::UBTTask_InitTargetLoc(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Initialise TargetLoc";
}

EBTNodeResult::Type UBTTask_InitTargetLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if (auto* const Controller = Cast<ATankAIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* const npc = Controller->GetPawn())
		{
			FVector TargetLocation = {npc->GetActorLocation().X + StartOffset,npc->GetActorLocation().Y,npc->GetActorLocation().Z};
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",TargetLocation);
			return  EBTNodeResult::Succeeded;
		}
		
	}
	return EBTNodeResult::Type();
}
