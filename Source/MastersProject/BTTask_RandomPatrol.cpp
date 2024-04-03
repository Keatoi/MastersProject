// Masters Project - Owen S Atkinson


#include "BTTask_RandomPatrol.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "TankAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandomPatrol::UBTTask_RandomPatrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Search area randomly within Radius";
}

EBTNodeResult::Type UBTTask_RandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if(auto* const controller = Cast<ATankAIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* const npc = controller->GetPawn())
		{
			//get npc location as random location origin
			auto const Origin = npc->GetActorLocation();
			//get nav system and generate random location
			if(auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Location;
				if(NavSys->GetRandomReachablePointInRadius(Origin,PatrolRadii,Location))
				{
					FVector TargetLocation = {Location.Location.X + 3000.f,Location.Location.Y + 3000.f,0.f};
					OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",TargetLocation);
					UE_LOG(LogTemp,Warning,TEXT("Location Saved"));
					UE_LOG(LogTemp,Warning,TEXT("Target Location: %s"),*TargetLocation.ToString());
					//Finish successfully
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
					
				}
				
			}
			
		}
		return EBTNodeResult::Failed;
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
