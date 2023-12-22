// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShrapnelComponent.generated.h"

/**
 *
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERSPROJECT_API UShrapnelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShrapnelComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void ShrapnelFire(int ShrapnelNum, float HalfAngle);
	
	
};
