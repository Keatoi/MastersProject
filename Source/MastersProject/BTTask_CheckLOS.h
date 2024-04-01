// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckLOS.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API UBTTask_CheckLOS : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_CheckLOS(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
