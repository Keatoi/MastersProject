// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "ChaosTankPawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_ChaosTank.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API AAI_ChaosTank : public AChaosTankPawn
{
	GENERATED_BODY()
public:
	UBehaviorTree* GetBehaviourTree() const;//BT getter for AI Controller
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
};
