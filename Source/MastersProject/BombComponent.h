// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UParticleSystem* FireballEmitter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* BombSystem;

	UPROPERTY()
	UNiagaraComponent* BombInstance;
	
	UFUNCTION(BlueprintCallable)
	void CreateFireball(FVector Location, float range = 50.f,float strength = 250.f);
	
		
};
