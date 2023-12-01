// Fill out your copyright notice in the Description page of Project Settings.
/* Generic Projectile Base Class
 * This class should be treated as a template for all projectiles that are fired from a cannon or a gun.
 * Rockets, Bombs and Sabot type projectiles will likely get their own base classes due to their differences to
 * traditional projectiles.
 *
 * ======Important Variables======
 * Explosive Filler - the amount of explosives inside the projectile in Kg
 * bUsePenOverride & PenOverride - instead of calculating the amount of armour penetration, you can instead manually
 * enter the amount (in mm)
 * Mass  - Mass of the projectile in Kg
 * bSloMoMode = If enabled physically slows down the projectile, and inputs the maximum velocity into the penetration
 * calculations, If not enabled the velocity used in the calculations is the projectiles current velocity
 * bCreatesShrapnel - If true and a shrapnel component is attached then after hit several smaller projectiles are dispersed
 * in random directions
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class MASTERSPROJECT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* Shell;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float ExplosiveFiller = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float Mass = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	FVector InitialLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	FVector Velocity;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float Force;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	//how much force should be lost over time since firing
	float ForceLoss;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float DeltaT;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	// I am using uint8 instead of bool as it saves a tiny bit of memory and functions exactly the same
	uint8 bForceLookForward:1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	//Amount to offset the z axis by assuming bApplyGravity is true
	float Gravity;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	//Set to True if gravity need be applied
	uint8 bApplyGravity:1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float PenetrationForceLoss;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Properties")
	float BaseDamage;
	
	/*==================================DEBUG VARS================================================*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", meta = (ExposeOnSpawn = true))
	uint8 bUsePenOverride:1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", meta = (ExposeOnSpawn = true))
	float PenetrationOverride = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", meta = (ExposeOnSpawn = true))
	bool bSloMoMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", meta = (ExposeOnSpawn = true))
	//mark hits with a sphere
	bool bMarkHits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", meta = (ExposeOnSpawn = true))
	// draw a line of travel 
	bool bMarkPath;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", meta = (ExposeOnSpawn = true))
	//prints a string for penetration calcs
	bool bPenDebug;
	UFUNCTION()
	void Move();
private:
	float InitialForce;
protected:
	void SetInitialPosition();
	void SetInitialVelocity();
	
	void ApplyGravity();
	void ForceLossFunc();
	void ForceLookForward();

	
	
	
	
};
