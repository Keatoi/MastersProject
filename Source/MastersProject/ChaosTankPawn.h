// Masters Project - Owen S Atkinson

#pragma once
#include "DamageInterface.h"
#include "BaseProjectile.h"
#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Components/TimelineComponent.h"
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
	EENGINEIDLE UMETA(DisplayName = "Idle Engine"),
	EENGINENOTIDLE UMETA(DisplayName = "Active Engine"),
	EENGINEBROKE UMETA(DisplayName = "Broken Engine")
};
UENUM(BlueprintType)
enum EAmmoEnum
{
	EAMMONORMAL UMETA(DisplayName = "Ammo Normal"),
	EAMMODESTROYED UMETA(DisplayName = "Ammo Detonation")
};
UENUM(BlueprintType)
enum EGunBreechEnum
{
	EBREECHNORMAL UMETA(DisplayName = "Breech Normal"),
	EBREECHDAMAGED UMETA(DisplayName = "Breech Damaged"),
	EBREECHBROKE UMETA(DisplayName = "Breech Broke")
};
UENUM(BlueprintType)
enum ECannonEnum
{
	ECANNONNORMAL UMETA(DisplayName = "Cannon Normal"),
	ECANNONBROKE UMETA(DisplayName = "Cannon Broke")
	
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	//Input Maps
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputTurn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	 UInputAction* InputFirePrimary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputFireSecondary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputReloadIM;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
	FName Team = "Blue";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterial* BaseMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterial* ClearMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	uint8 bUseClearMesh:1;
	//==================Dynamic Materials================================
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInstanceDynamic* DynamicLeftTrack;
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInstanceDynamic* DynamicRightTrack;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ENUMS", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EEngineEnum> EngineEnum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ENUMS", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAmmoEnum> AmmoStowageEnum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ENUMS", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EGunBreechEnum> BreechEnum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ENUMS", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECannonEnum> CannonEnum;
	//==================Projectiles================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> MachineGunProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	float ReloadTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	int MGMagazine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	int MGMagCapacity;//How much projectiles the MG should reload
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	float InteriorMagazine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	float InteriorCapacity = 5.f;//Maximum amount of shells to store in the Interior Magazine
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	float InteriorReloadDelay;//How many seconds between adding each shell to interior magazine
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	float AmmoReserve;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles", meta = (AllowPrivateAccess = "true"))
	float MGReload = 3.5f;//How many seconds to reload MG
	//=================Sound Variables===========
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_Engine;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_Engine_Idle;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_MainGun;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_MachineGun;
	UPROPERTY(EditAnywhere, Category="Sounds")
	USoundBase* SB_Turret;
	//================Visual Effects============
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	class UNiagaraSystem* MuzzleSystem;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	class UNiagaraComponent* MuzzleInstance;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	UNiagaraSystem* DeathSystem;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	 UNiagaraComponent* DeathInstance;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	USceneComponent* MuzzleFlashComponent; //Where the MuzzleFlash will be spawned
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	float MuzzleCoefStrength;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	UNiagaraSystem* ExhaustSystem;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	UNiagaraComponent* ExhaustInstance;
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	USceneComponent* EngineExhaustComponent; 
	UPROPERTY(EditAnywhere, Category = "Effects and Spawners")
	float ExhaustCoefStrength;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effects and Spawners", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* MS_Turbine;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Effects and Spawners")
	float EngineRPM;
	//=================Misc. Variables===========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	FName TankName = "T-72(A)";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	float TurnLimit = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	float TurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	uint8 bStopTurn:1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	uint8 bFreeLookEnabled:1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	int TankHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Statistics", meta = (AllowPrivateAccess = "true"))
	FName TurretBone = "turret_jnt";
	//==================UX Func/Var===================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank UX", meta = (AllowPrivateAccess = "true"))
	FVector ScreenVector;
	UFUNCTION(Blueprintable)
	FVector GetGunSightScreenPos();
protected:
	//==============Timers======================
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimeLine;
	FOnTimelineEvent TimeLineUpdateEvent;
	UPROPERTY()
	UCurveFloat* DetonateCurve;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle ReloadTimerHandle;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle MGFireRateHandle;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle IMDelayHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bCanShoot:1;
public:
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
	void SecondaryFireStart(const FInputActionValue &Value);
	UFUNCTION()
	void SecondaryFire();
	UFUNCTION()
	void SecondaryFireReleased(const FInputActionValue &Value);
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
	UFUNCTION()
	void HealthCheck();
	UFUNCTION()
	void Detonate();
	UFUNCTION()
	void TurretDetonationImpulse();
	UFUNCTION()
	virtual void SetHitComponent_Implementation(USceneComponent* HitComponent) override;
	UFUNCTION()
	void Reload();//Reload Gun after every shot
	UFUNCTION()
	void ReloadMG();
	UFUNCTION()
	void ReloadInteriorMagazine(const FInputActionValue& Value);//Reload the InteriorMagazine
	//====Helper Functions===
	UFUNCTION()
	void SetMatScalarSpeed(int Index,float Speed);
	
};
