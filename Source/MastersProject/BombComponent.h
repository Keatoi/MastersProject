// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BombComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERSPROJECT_API UBombComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBombComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* FireBallCollision;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UShrapnelComponent* ShrapnelComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RadiusIncrement = 1.f;
	UFUNCTION(BlueprintCallable)
	void CreateFireball(float range,float strength,FVector Location);
	
		
};
