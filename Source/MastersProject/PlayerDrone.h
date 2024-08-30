// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerDrone.generated.h"

UCLASS()
class MASTERSPROJECT_API APlayerDrone : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerDrone();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* DroneMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* FollowCam;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* DroneCam;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector CurrLocation;
	UPROPERTY()
	float Accel;
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float Speed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float AccelRate = 2000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float InterpSpeed = 10.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float MaxRange = 40000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Missle")
	float BatteryLifespan = 40000.f;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
