// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_VehicleMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API UBTTask_VehicleMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_VehicleMoveTo(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed = 1.5f;
};
