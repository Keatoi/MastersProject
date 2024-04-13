// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "GuidedInterface.h"
#include "GuidedMissile.generated.h"


/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API AGuidedMissile : public ABaseProjectile, public IGuidedInterface
{
	GENERATED_BODY()
protected:
	AGuidedMissile();
	virtual void Tick(float DeltaTime) override;
	virtual void Launch(FVector MoveDirection) override;
	UPROPERTY()
	uint8 bHasTarget:1 = false;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float MissileLaunchDistance = 500.f;//How many uu to launch the missle upwards;
};
