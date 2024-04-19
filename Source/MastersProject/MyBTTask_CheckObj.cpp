// Masters Project - Owen S Atkinson


#include "MyBTTask_CheckObj.h"

#include "AI_ChaosTank.h"
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
			if (npc->IsA<AAI_ChaosTank>())
			{
				//Movement Code
				//Cast To AITank
				AAI_ChaosTank* tank = Cast<AAI_ChaosTank>(npc);
				if(npc->ActorHasTag("Red"))
				{
					for(int i =0; i <Arr.Num();i++)
					{
						AObjectiveactor* ObjAct = Cast<AObjectiveactor>(Arr[i]);
						UE_LOG(LogTemp,Warning,TEXT("Objective found"));
				
						if(ObjAct->CaptureTeamEnum != ERED && MathHelper::GetDistance(npc,ObjAct) <= AcceptableDistance)
						{
							FVector TargetLocation = ObjAct->GetActorLocation();
							OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",TargetLocation);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool("bShouldCapture",true);
							UE_LOG(LogTemp,Warning,TEXT("Objective found"));
						}
						else
						{
							UE_LOG(LogTemp,Warning,TEXT("Objective Invalid"));
					
							OwnerComp.GetBlackboardComponent()->SetValueAsBool("bShouldCapture",false);
							FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
							return EBTNodeResult::Failed;
						}
						FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
				else
				{
					for(int i =0; i <Arr.Num();i++)
					{
						AObjectiveactor* ObjAct = Cast<AObjectiveactor>(Arr[i]);
						UE_LOG(LogTemp,Warning,TEXT("Objective found"));
				
						if(ObjAct->CaptureTeamEnum != EBLU && MathHelper::GetDistance(npc,ObjAct) <= AcceptableDistance)
						{
							FVector TargetLocation = ObjAct->GetActorLocation();
							OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation",TargetLocation);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool("bShouldCapture",true);
							UE_LOG(LogTemp,Warning,TEXT("Objective found"));
						}
						else
						{
							UE_LOG(LogTemp,Warning,TEXT("Objective Invalid"));
					
							OwnerComp.GetBlackboardComponent()->SetValueAsBool("bShouldCapture",false);
							FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
							return EBTNodeResult::Failed;
						}
						FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
				
			}
			
			return EBTNodeResult::Failed;
		}
		
	}
	return EBTNodeResult::Type();
}
