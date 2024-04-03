// Masters Project - Owen S Atkinson


#include "MyBTTask_CheckObj.h"
#include "MathHelper.h"
#include "Objectiveactor.h"
#include "TankAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UMyBTTask_CheckObj::UMyBTTask_CheckObj(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check Objective Status";
}

EBTNodeResult::Type UMyBTTask_CheckObj::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ATankAIController>(OwnerComp.GetAIOwner()))
	{
		if(auto* const npc = Controller->GetPawn())
		{
			TArray<AActor*>Arr;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(),AObjectiveactor::StaticClass(),Arr);
			for(int i =0; i <Arr.Num();i++)
			{
				AObjectiveactor* ObjAct = Cast<AObjectiveactor>(Arr[i]);
				UE_LOG(LogTemp,Warning,TEXT("Objective found"));
				
				if(ObjAct->CaptureTeamEnum != ERED && MathHelper::GetDistance(npc,ObjAct) <= AcceptableDistance)
				{
					FVector TargetLocation = ObjAct->GetActorLocation();
					OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",TargetLocation);
					UE_LOG(LogTemp,Warning,TEXT("Moving to Obj"));
				}
				else
				{
					UE_LOG(LogTemp,Warning,TEXT("Objective Invalid"));
				}
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			return EBTNodeResult::Failed;
		}
		
	}
	return EBTNodeResult::Type();
}
