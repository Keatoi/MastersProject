// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_InitTargetLoc.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API UBTTask_InitTargetLoc : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	explicit UBTTask_InitTargetLoc(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta=(AllowPrivateAccess="true"))
	double StartOffset = 5000;
};
