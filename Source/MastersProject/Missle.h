// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

#include "Missle.generated.h"

UCLASS()
class MASTERSPROJECT_API AMissle : public AActor
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float Speed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float AccelRate = 2000;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float InterpSpeed = 10;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	AActor* TargetActor;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	TEnumAsByte<ECollisionChannel> ColChannel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	uint8 bDebug:1 = false;
	UFUNCTION(BlueprintCallable,Category = "Missles")
	void Launch(FVector Direction);
	UFUNCTION(BlueprintCallable,Category = "Missles")
	void SetTarget(AActor* Target);
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void MissleRot(float RotSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
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
