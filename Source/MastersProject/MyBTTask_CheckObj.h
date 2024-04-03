// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_CheckObj.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API UMyBTTask_CheckObj : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UMyBTTask_CheckObj(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	float AcceptableDistance = 50000.f;
};
