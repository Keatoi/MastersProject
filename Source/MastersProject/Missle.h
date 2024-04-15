// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GuidedInterface.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

#include "Missle.generated.h"

UCLASS()
class MASTERSPROJECT_API AMissle : public AActor, public IGuidedInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissle();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	UStaticMeshComponent* SM;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	USphereComponent* CollisionSphere;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Niagara")
	UNiagaraSystem* PlumeSystem;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Niagara")
	UNiagaraComponent* PlumeComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBombComponent* HEComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float Speed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float AccelRate = 2000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float InterpSpeed = 10.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float MaxRange = 40000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float MinRange = 20000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float ExplosiveFiller = 12000.f;//in grams
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float DetectRange = 500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float FuseTimer = 20.f;//Time to self-destruct if enabled
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	FVector TargetLoc;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	TEnumAsByte<ECollisionChannel> ColChannel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	uint8 bDebug:1 = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	uint8 bSelfDestruct:1 = false;//If true Detonate after x Seconds
	UFUNCTION(BlueprintCallable,Category = "Missles")
	void Launch(FVector Direction);
	UFUNCTION(BlueprintCallable,Category = "Missles")
	void SetTarget();
	UFUNCTION()
	void ProximityCheck();
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void MissleRot();
	UFUNCTION()
	void Detonate();
	UFUNCTION()
	virtual void SetTargetLocation_Implementation(FVector TargetLocation) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;// I saw somewhere on the internet that this helps with rare Tick bugs, not sure if true so only using it in protected in this class
private:
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector CurrLocation;
	UPROPERTY()
	float Accel;
	UPROPERTY()
	FTimerHandle MissleHandle;
	UPROPERTY()
	uint8 bIsSafe:1;//is the missile far enough away to not cause significant harm to Owner

};
