// Masters Project - Owen S Atkinson


#include "TankAIController.h"

#include "AI_ChaosTank.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ATankAIController::ATankAIController(FObjectInitializer const& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = false;
//Configure Hearing Senses
	AISenseConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	AISenseConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigHearing->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigHearing->DetectionByAffiliation.bDetectNeutrals = false;
//Set sight as dominant sight
	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AAIController::SetGenericTeamId(TeamID);
	//add dynamic delegate for target perception
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated_Delegate);
}

ETeamAttitude::Type ATankAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	//If detected pawn has the team interface, check if its on the same team or not and change attitude accordingly. If no interface set to neutral
	if(APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if(auto const TeamInterface = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if(TeamInterface->GetGenericTeamId()== FGenericTeamId(TeamID))
			{
				return ETeamAttitude::Friendly;
			}
			return ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}

void ATankAIController::OnTargetPerceptionUpdated_Delegate(AActor* Actor, FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
	 // react to sight stimulus
	case 1:
	 // react to hearing
	default:
	 return;
	}
}

void ATankAIController::OnPossess(APawn* InPawn)
{
	//Set BB if controller attached to AI tank
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
