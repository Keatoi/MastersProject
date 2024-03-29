// Masters Project - Owen S Atkinson


#include "BTTask_VehicleMoveTo.h"
#include "AI_ChaosTank.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "TankAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
UBTTask_VehicleMoveTo::UBTTask_VehicleMoveTo(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Move To Specified Location";
}

EBTNodeResult::Type UBTTask_VehicleMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const Controller = Cast<ATankAIController>(OwnerComp.GetAIOwner()))
	{
		if (auto* const Npc = Controller->GetPawn())
		{
			//get npc location and Target Location
			auto const NpcLoc = Npc->GetActorLocation();
			auto const TargetLocation = Controller->GetBlackboardComponent()->GetValueAsVector(FName("TargetLocation"));
			//Get Look At Rotation and DeltaTime the RInterp the rotations
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(NpcLoc, TargetLocation);
			float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(Npc);
			FRotator InterpRot = FMath::RInterpTo(Npc->GetActorRotation(), LookAtRotation, DeltaTime, InterpSpeed);
			
			Npc->SetActorRotation(InterpRot,ETeleportType::TeleportPhysics);
			if (Npc->IsA<AAI_ChaosTank>())
			{
				//Movement Code
				//Cast To AITank
				Controller->MoveToLocation(TargetLocation,AcceptanceRadius,true,true,false,false);
				UE_LOG(LogTemp,Warning,TEXT("Moving To: %s"),*TargetLocation.ToString());

			}
			//Finish successfully
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			

		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Type();
}
