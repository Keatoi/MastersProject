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
	AAI_ChaosTank();
	virtual void BeginPlay() override;
	UBehaviorTree* GetBehaviourTree() const;//BT getter for AI Controller
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI", meta=(AllowPrivateAccess="true"))
	class UPawnSensingComponent* PawnSense;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Pathing", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class USplineComponent> Spline;
	/////////////////////////////////////////////////////////////////////////
	// UPawnSensingComponent Delegates
public:
	UFUNCTION()
	void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);

	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);
	UFUNCTION()
	void SetThrottle(float Throttle);
	UFUNCTION()
	void SetBrake(float Brake);
	UFUNCTION()
	void Pathfinding();
};
