// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API UBTTask_RandomPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_RandomPatrol(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta=(AllowPrivateAccess="true"))
		float PatrolRadii = 10000.f;
};
