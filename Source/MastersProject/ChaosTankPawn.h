// Masters Project - Owen S Atkinson

#pragma once
#include "DamageInterface.h"
#include "BaseProjectile.h"
#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ChaosTankPawn.generated.h"
UENUM(BlueprintType)
enum ECameraType
{
	EDEFAULTCAM UMETA(DisplayName = "Default Camera"),
	ECOMMANDERCAM UMETA(DisplayName = "Commander Camera"),
	EGUNNERCAM UMETA(DisplayName = "Gunner Camera")
};
UENUM(BlueprintType)
enum EEngineEnum
{
	EENGINEIDLE,
	EENGINENOTIDLE,
	EENGINEBROKE
};
/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API AChaosTankPawn : public AWheeledVehiclePawn, public IDamageInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AChaosTankPawn();

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
	APlayerCameraManager* CamManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	 UCameraComponent* ZoomCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* GunnerCamera;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;//Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputTurn;
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
	//==================Interior Components==============================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior Components")
	UStaticMeshComponent* EngineBlock;//If Hit Engine Dies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior Components")
	class UBoxComponent* EngineBlockCollider;//Back up incase SniperElite Style System does not work
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior Components")
	UStaticMeshComponent* GunBreech;//If Hit Cant Reload
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior Components")
	UStaticMeshComponent* AmmoStowage;//If Hit explode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior Components")
	UStaticMeshComponent* FuelTank;//If Hit explode
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
	//==================DamageEnum================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EEngineEnum> EngineEnum;
	//==================Projectiles================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> MachineGunProjectileClass;
	//=================Sound Variables===========
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_Engine;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_MainGun;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_MachineGun;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_Turret;
	//================Visual Effects============
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	class UNiagaraSystem* FireEffectMuzzle;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	USceneComponent* MuzzleFlashComponent; //Where the MuzzleFlash will be spawned
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	float MuzzleCoefStrength;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	class UNiagaraSystem* EngineEffectExhaust;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	USceneComponent* EngineExhaustComponent; 
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	float ExhaustCoefStrength;
	//=================Misc. Variables===========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	float TurnLimit = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	float TurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	uint8 bStopTurn:1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	uint8 bFreeLookEnabled:1;
	//==================UX Func/Var===================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank UX", meta = (AllowPrivateAccess = "true"))
	FVector ScreenVector;
	UFUNCTION(Blueprintable)
	FVector GetGunSightScreenPos();
	
	//=================Input Functions=============
	UFUNCTION()
	void MoveTriggered(const FInputActionValue &Value);
	UFUNCTION()
	void MoveCompleted(const FInputActionValue &Value);
	UFUNCTION()
	void MoveStarted(const FInputActionValue &Value);
	UFUNCTION()
	void MoveCancelled(const FInputActionValue &Value);
	UFUNCTION()
	void TurnTriggered(const FInputActionValue &Value);
	UFUNCTION()
	void TurnStarted(const FInputActionValue &Value);
	UFUNCTION()
	void TurnCancelled(const FInputActionValue &Value);
	UFUNCTION()
	void TurnCompleted(const FInputActionValue &Value);
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
	UFUNCTION()
	void OnTankHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
};
