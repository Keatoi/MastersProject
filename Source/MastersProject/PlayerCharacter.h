// Masters Project - Owen S Atkinson
//This character is simply for testing the projectiles
#pragma once
#include "BaseProjectile.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"
UENUM(BlueprintType)
enum ECameraType
{
	EDEFAULTCAM UMETA(DisplayName = "Default Camera"),
	ECOMMANDERCAM UMETA(DisplayName = "Commander Camera"),
	EGUNNERCAM UMETA(DisplayName = "Gunner Camera")
};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* TestPlayerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	 UCameraComponent* ZoomCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* GunnerCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;//Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	 UInputAction* InputFirePrimary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	 UInputAction* InputLook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputCameraSwap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputDefaultCam;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputZoomCam;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputGunnerCam;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECameraType> CamEnum;
	 //=============Turret Controls=========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float TurretTraverse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float TurretTraverseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float MGTraverse;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float MGTraverseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float TurretElevation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float TurretElevationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float MGElevation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float MGElevationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> MachineGunProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	FVector ProjectileSpawnOffset;
	//=================Input Functions=============
	UFUNCTION()
	void Move(const FInputActionValue &Value);
	UFUNCTION()
	void Look(const FInputActionValue &Value);
	UFUNCTION()
	void PrimaryFire(const FInputActionValue &Value);
	UFUNCTION()
	void SecondaryFire(const FInputActionValue &Value);
	UFUNCTION()
	void CameraSwap(const FInputActionValue &Value);
	UFUNCTION()
	void DefaultView(const FInputActionValue &Value);
	UFUNCTION()
	void CommanderView(const FInputActionValue &Value);
	UFUNCTION()
	void GunnerView(const FInputActionValue &Value);
	
	
};
