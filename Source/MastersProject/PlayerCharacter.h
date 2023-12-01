// Masters Project - Owen S Atkinson
//This character is simply for testing the projectiles
#pragma once
#include "BaseProjectile.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MASTERSPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	class UCameraComponent* TestPlayerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;//Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	 UInputAction* InputFirePrimary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	 UInputAction* InputLook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	USceneComponent* PrimaryWeaponLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SecondaryWeaponLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	FVector ProjectileSpawnOffset;
	UFUNCTION()
	void Move(const FInputActionValue &Value);
	UFUNCTION()
	void Look(const FInputActionValue &Value);
	UFUNCTION()
	void PrimaryFire(const FInputActionValue &Value);
	//collision
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	
};
