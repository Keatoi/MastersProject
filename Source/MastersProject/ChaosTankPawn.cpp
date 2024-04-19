// Masters Project - Owen S Atkinson


#include "ChaosTankPawn.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MathHelper.h"
#include "Missle.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Channels/MovieSceneDoubleChannel.h"
#include "Components/AudioComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
/*Tank Values Based on T-72 Soviet MBT, may need tweaking if changing Skeleton Mesh to better reflect their real life counter part*/
AChaosTankPawn::AChaosTankPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//=====Cam Setup======
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetMesh(),"TurretSocket");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->TargetArmLength = 1000.0f;
	TestPlayerCamera = CreateDefaultSubobject<UCameraComponent>("Cam");
	TestPlayerCamera->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	ZoomCamera = CreateDefaultSubobject<UCameraComponent>("Commander Cam");
	ZoomCamera->SetupAttachment(GetMesh(),"CommanderCamSocket");
	ZoomCamera->SetRelativeLocationAndRotation(FVector::ZeroVector,FRotator::ZeroRotator);
	ZoomCamera->SetActive(false);
	GunnerCamera = CreateDefaultSubobject<UCameraComponent>("Gunner Cam");
	GunnerCamera->SetupAttachment(GetMesh(),"GunCamSocket");
	GunnerCamera->SetRelativeLocationAndRotation(FVector::ZeroVector,FRotator::ZeroRotator);
	GunnerCamera->SetActive(false);
	CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>("PPComp");
	PostProcessComponent->SetupAttachment(GetMesh());
	
	
	/*Interior Damage Mesh Set Up
	 * Each Component needs a Static Mesh, a Socket to attach to and an Tag,
	 * The projectile will MultiTrace upon impact and if it hits an component it will have the appropriate effect here,
	 * likely with an Interface
	 */
	EngineBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Engine Block"));
	EngineBlock->ComponentTags.Add(FName("Engine"));
	EngineBlock->SetupAttachment(GetMesh(),"EngineBlockSocket");

	GunBreech = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Breech"));
	GunBreech->ComponentTags.Add(FName("GunBreech"));
	GunBreech->SetupAttachment(GetMesh(),"GunBreechSocket");

	AmmoStowage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Stowage"));
	AmmoStowage->ComponentTags.Add(FName("AmmoStowage"));
	AmmoStowage->SetupAttachment(GetMesh(),"AmmoStowageSocket");

	FuelTank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fuel Tank"));
	FuelTank->ComponentTags.Add(FName("FuelTank"));
	FuelTank->SetupAttachment(GetMesh(),"FuelTankSocket");
	//MetaSounds Audio
	MS_Turbine = CreateDefaultSubobject<UAudioComponent>(TEXT("MS_Engine"));
	/*Timeline setup 
	 */
	
	bCanShoot = true;//Ensure we can shoot at spawn
	Tags.Add(Team);//Add actor Tag for team detection purposes
	
	
}

void AChaosTankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	bStopTurn = false;
	OnActorHit.AddDynamic(this,&AChaosTankPawn::OnTankHit);
	//Set Dynamic Material Instances and start speed at 0.f so it isn't moving
	DynamicLeftTrack = GetMesh()->CreateAndSetMaterialInstanceDynamic(1);
	DynamicRightTrack = GetMesh()->CreateAndSetMaterialInstanceDynamic(2);
	SetMatScalarSpeed(2,0.f);
	if(MS_Turbine)
	{
		MS_Turbine->Activate();
		MS_Turbine->Play();
		
	}
	if(ExhaustSystem)
	{
		//Engine Exhaust.Doesn't auto destroy since we want this to be on until tank is destroyed
		FVector Scale = {1,1,1};
		ExhaustInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),ExhaustSystem,EngineBlock->GetComponentLocation(),FRotator::ZeroRotator,Scale,false);
		ExhaustInstance->Activate();
	}
}

void AChaosTankPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Clear Timers if actor removed from level (i.e destroyed or despawned)
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	if(ExhaustInstance){ExhaustInstance->Deactivate();}
	
}

void AChaosTankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float AngularVectorLength = GetMesh()->GetPhysicsAngularVelocityInDegrees().Length();
	if(AngularVectorLength > TurnLimit)
	{
		bStopTurn = true;
	}
	else
	{
		bStopTurn = false;
	}
	ScreenVector = GetGunSightScreenPos();
	if(Missile != nullptr)
	{
		if(Missile->GetClass()->ImplementsInterface(UGuidedInterface::StaticClass()))
		{
			Execute_SetTargetLocation(Missile,ScreenVector);
		}
		
	}
	
	//Apply parameter to Metasound graph to change pitch and mix between idle and move soundcues
	if(MS_Turbine)
	{
		FVector2D InRange(-30.f,30.f);
		FVector2D OutRange(0.f,1.f);
		//EngineRPM = FMath::GetMappedRangeValueClamped(InRange,OutRange,UKismetMathLibrary::Abs(GetVehicleMovement()->GetForwardSpeedMPH()));
		EngineRPM = GetVehicleMovement()->GetForwardSpeedMPH();
		MS_Turbine->SetFloatParameter("input",EngineRPM);
		
		//UE_LOG(LogTemp, Display, TEXT("look value: %f"), EngineRPM);
	}
}

void AChaosTankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Get Player Controller and Enhanced Input Subsystem
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	const auto eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	eiSubsystem->AddMappingContext(InputMapping, 0);
	UE_LOG(LogTemp,Warning,TEXT("EIC loaded"));
	//Bind Actions to functions
	UEnhancedInputComponent* Ei = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Ei->BindAction(InputMove,ETriggerEvent::Triggered,this,&AChaosTankPawn::MoveTriggered);
	Ei->BindAction(InputMove,ETriggerEvent::Completed,this,&AChaosTankPawn::MoveCompleted);
	Ei->BindAction(InputMove,ETriggerEvent::Canceled,this,&AChaosTankPawn::MoveCancelled);
	Ei->BindAction(InputMove,ETriggerEvent::Started,this,&AChaosTankPawn::MoveStarted);
	Ei->BindAction(InputTurn,ETriggerEvent::Triggered,this,&AChaosTankPawn::TurnTriggered);
	Ei->BindAction(InputTurn,ETriggerEvent::Started,this,&AChaosTankPawn::TurnStarted);
	Ei->BindAction(InputTurn,ETriggerEvent::Canceled,this,&AChaosTankPawn::TurnCancelled);
	Ei->BindAction(InputTurn,ETriggerEvent::Completed,this,&AChaosTankPawn::TurnCompleted);
	Ei->BindAction(InputLook,ETriggerEvent::Triggered,this,&AChaosTankPawn::Look);
	Ei->BindAction(InputFirePrimary,ETriggerEvent::Started,this,&AChaosTankPawn::PrimaryFire);
	Ei->BindAction(InputFireSecondary,ETriggerEvent::Started,this,&AChaosTankPawn::SecondaryFireStart);
	Ei->BindAction(InputMissleToggle,ETriggerEvent::Started,this,&AChaosTankPawn::MissileToggle);
	Ei->BindAction(InputReloadIM,ETriggerEvent::Started,this,&AChaosTankPawn::ReloadInteriorMagazine);
	Ei->BindAction(InputCameraSwap,ETriggerEvent::Started,this,&AChaosTankPawn::CameraSwap);
	Ei->BindAction(InputDefaultCam,ETriggerEvent::Started,this,&AChaosTankPawn::DefaultView);
	Ei->BindAction(InputZoomCam,ETriggerEvent::Started,this,&AChaosTankPawn::CommanderView);
	Ei->BindAction(InputGunnerCam,ETriggerEvent::Started,this,&AChaosTankPawn::GunnerView);
}




FVector AChaosTankPawn::GetGunSightScreenPos()
{
	//Trace then project to screen for UI Location
	FHitResult SightHit;
	FVector GunLocation = GetMesh()->GetSocketLocation("Main_CaliberSocket");
	FVector GunFWD = GetMesh()->GetSocketRotation("Main_CaliberSocket").Vector() * 25000.f;
	FVector TraceEnd = GunLocation + GunFWD;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray; // object types to trace
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));
	TArray<AActor*> IgnoredActorsArray;
	bool bTraceSuccessful = false;
	if(UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),GunLocation,TraceEnd,ObjectTypesArray,false,IgnoredActorsArray,EDrawDebugTrace::None,SightHit,true,FLinearColor::Red,FLinearColor::Blue,5.0f))
	{
		bTraceSuccessful = true;
	}
	//if we hit a trace location then return the hit location else return the end of the trace
	FVector TraceLocation = bTraceSuccessful ? SightHit.Location : TraceEnd;
	return TraceLocation;
}

void AChaosTankPawn::MoveTriggered(const FInputActionValue& Value)
{
	//movement code
	
	const FVector2d MoveValue = Value.Get<FVector2d>();
	//If Engine broke cancel move
	if(EngineEnum == EENGINEBROKE)
	{
		MoveCancelled(Value);
	}
	//if engine functional, adjust throttle and brake input to get desired movement
	else if(MoveValue.Y >= 0.0f)
	{
		
			GetVehicleMovement()->SetThrottleInput(MoveValue.Y);
			GetVehicleMovement()->SetBrakeInput(0.f);
			
		
	}
	else if (MoveValue.Y < 0.0f)
	{
		GetVehicleMovement()->SetThrottleInput(MoveValue.Y);
		GetVehicleMovement()->SetBrakeInput(MoveValue.Y * -1.f);
	}
	//Move tracks
	SetMatScalarSpeed(2,MoveValue.Y);
	
}

void AChaosTankPawn::MoveCompleted(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Display, TEXT("move value: %f"), Value.Get<float>());
	const FVector2d MoveValue = Value.Get<FVector2d>();
	if(MoveValue.Y >= 0.0f)
	{
		
		GetVehicleMovement()->SetThrottleInput(MoveValue.Y);
		GetVehicleMovement()->SetBrakeInput(0.f);
	}
	else if (MoveValue.Y <= 0.0f)
	{
		GetVehicleMovement()->SetThrottleInput(MoveValue.Y );
		GetVehicleMovement()->SetBrakeInput(MoveValue.Y * -1.f);
	}
	SetMatScalarSpeed(2,MoveValue.Y);
}

void AChaosTankPawn::MoveStarted(const FInputActionValue& Value)
{
	
	GetVehicleMovement()->SetYawInput(0.f);
	GetVehicleMovement()->SetThrottleInput(0.f);
	//SetMatScalarSpeed(2,0.f);
}

void AChaosTankPawn::MoveCancelled(const FInputActionValue& Value)
{
	//set everything to 0.f;
	GetVehicleMovement()->SetYawInput(0.f);
	GetVehicleMovement()->SetThrottleInput(0.f);
	SetMatScalarSpeed(2,0.f);
}

void AChaosTankPawn::TurnTriggered(const FInputActionValue& Value)
{
	if(bStopTurn)
	{
		//come to smooth stop if turn threshold hit
		TurnRate = UKismetMathLibrary::Lerp(TurnRate,0.f,0.1f);
		GetVehicleMovement()->SetYawInput(TurnRate);
	}
	else
	{
		//turn right or left
		TurnRate = Value.Get<float>();
		if(TurnRate > 0.f){SetMatScalarSpeed(1,TurnRate * 25.f);}
		else if(TurnRate < 0.f){SetMatScalarSpeed(0,TurnRate * 25.f);}
		GetVehicleMovement()->SetYawInput(TurnRate);
	}
	
}

void AChaosTankPawn::TurnStarted(const FInputActionValue& Value)
{
	//Needs a tiny bit of throttle to turn, dont want to add too much in case the tank is moving(this could cause the tank to roll/flip/spin out of control)
	GetVehicleMovement()->SetThrottleInput(0.05f);
}

void AChaosTankPawn::TurnCancelled(const FInputActionValue& Value)
{
	GetVehicleMovement()->SetYawInput(0.f);
	GetVehicleMovement()->SetThrottleInput(0.f);
	SetMatScalarSpeed(2,0.f);
}

void AChaosTankPawn::TurnCompleted(const FInputActionValue& Value)
{
	GetVehicleMovement()->SetYawInput(0.f);
	GetVehicleMovement()->SetThrottleInput(0.f);
	SetMatScalarSpeed(2,0.f);
}

void AChaosTankPawn::Look(const FInputActionValue& Value)
{
	//look around with mouse and turn turret to match mouse movement
	const FVector2D LookValue = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Display, TEXT("look value: %f"), Value.Get<float>());
	if (LookValue.X != 0.f)
	{
		//Turret traverse is different to camera control speed to ensure the turret movement isn't too fast/looks unnatural
		TurretTraverse += LookValue.X * 5.f;
		AddControllerYawInput(LookValue.X * 0.5);
	}
 
	if (LookValue.Y != 0.f)
	{
		TurretElevation += LookValue.Y * 10.f;
		//clamp Turret elevation so cannon doesnt clip through mesh, values taken from real world and slightly tweaked to prevent as much clipping as possible
		TurretElevation = FMath::Clamp(TurretElevation,-5.f,14.f);
		if(bFreeLookEnabled)
		{
			AddControllerPitchInput(LookValue.Y * 0.5);
		}
		
	}
}

void AChaosTankPawn::PrimaryFire(const FInputActionValue& Value)
{
	//If mouse pressed and there is no gun damage and we can shoot/arent reloading/aren't out of ammo
	if(const bool FireValue = Value.Get<bool>() &&(BreechEnum != EBREECHBROKE || CannonEnum != ECANNONBROKE) && bCanShoot && AmmoReserve > 0.f)
	{
		if(!ProjectileClass || !MissileClass)
		{
			// no projectile class set error prevention + on screen debug message
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("No Projectile Class Loaded!!!"));
		}
		else
		{
			
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Firing!!!"));
			//Spawn Parameters
			if(UWorld* World = GetWorld())
			{
				
				//Spawn Parameters
				FActorSpawnParameters SParams;
				SParams.Owner = this;
				SParams.Instigator = GetInstigator();
				//Spawn Projectile At location of the Main Barrels Socket
				FVector SpawnLocation = GetMesh()->GetSocketLocation("Main_CaliberSocket");
				FRotator SpawnRotation = GetMesh()->GetSocketRotation("Main_CaliberSocket");
				if(!bUseMissile)
				{
					//Remove Ammo from two-tier Magazines
					AmmoReserve--;
					if(InteriorMagazine > 0) InteriorMagazine--;//Ensure Interior Magazine does not go into the negatives
					ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass,SpawnLocation,SpawnRotation,SParams);//Spawn Projectile
					GetMesh()->AddImpulse(FVector(-150.f,0.f,0.f),FName(NAME_None),true);//Add Impulse to simulate recoil
				
					bCanShoot = false;//Disable firing until reloaded
					if(InteriorMagazine > 0.f)ReloadTime = 3.f; else ReloadTime = 5.f;//Set ReloadTime based on Two-tier magazine ammo
					World->GetTimerManager().SetTimer(ReloadTimerHandle,this,&AChaosTankPawn::Reload,ReloadTime,false);
					if(SB_MainGun){UGameplayStatics::PlaySoundAtLocation(World,SB_MainGun,SpawnLocation,SpawnRotation);}//Play Fire Sound if valid
				}
				else if (MissleMagazine > 0.f)
				{
					MissleMagazine--;
					
					Missile = World->SpawnActor<AMissle>(MissileClass,SpawnLocation,SpawnRotation,SParams);//Spawn Projectile
					GetMesh()->AddImpulse(FVector(-150.f,0.f,0.f),FName(NAME_None),true);//Add Impulse to simulate recoil
				
					bCanShoot = false;//Disable firing until reloaded
					ReloadTime = 10.f;//Set ReloadTime based on Two-tier magazine ammo
					World->GetTimerManager().SetTimer(ReloadTimerHandle,this,&AChaosTankPawn::Reload,ReloadTime,false);
					if(SB_MainGun){UGameplayStatics::PlaySoundAtLocation(World,SB_MainGun,SpawnLocation,SpawnRotation);}//TODO: Replace with missile sound
				}
				//Start reload sequence
				
				if (MuzzleSystem)
				{
					FVector Scale = {1,1,1};
					MuzzleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),MuzzleSystem,SpawnLocation,FRotator::ZeroRotator,Scale);
					MuzzleInstance->Activate();
				}
			}
		}
	}
	
}

void AChaosTankPawn::SecondaryFireStart(const FInputActionValue& Value)
{
	//Machine gun is an automatic weapon so works a little differently, we are using a timer to give us control over the Rate of Fire (ROF) of the gun.

		if(MGMagazine > 0)
		{
			//If we have bullets left in magazine start the MGTimer to fire every 0.1 sec
			GetWorld()->GetTimerManager().SetTimer(MGFireRateHandle,this,&AChaosTankPawn::SecondaryFire,0.1f,true);
		}
		
	
}

void AChaosTankPawn::SecondaryFire()
{
	if(!MachineGunProjectileClass)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("No Projectile Class Loaded!!!"));
	}
	else
	{
			
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Firing!!!"));
		//Spawn Parameters
		if(UWorld* World = GetWorld())
		{
			FActorSpawnParameters SParams;
			SParams.Owner = this;
			SParams.Instigator = GetInstigator();
			//Spawn Projectile At location of the Main Barrels Socket
			FVector SpawnLocation = GetMesh()->GetSocketLocation("MachineGunSocket");
			FRotator SpawnRotation = GetMesh()->GetSocketRotation("MachineGunSocket");
			ABaseProjectile* MachineGunProjectile= World->SpawnActor<ABaseProjectile>(MachineGunProjectileClass,SpawnLocation,SpawnRotation,SParams);
			MGMagazine--;
				
		}
	}
}

void AChaosTankPawn::SecondaryFireReleased(const FInputActionValue& Value)
{
	//Clear Timer on release to stop the gun firing
	GetWorldTimerManager().ClearTimer(MGFireRateHandle);
}

void AChaosTankPawn::MissileToggle(const FInputActionValue& Value)
{
	if(bUseMissile)
	{
		bUseMissile = false;
	}
	else
	{
		bUseMissile = true;
	}
}

void AChaosTankPawn::CameraSwap(const FInputActionValue& Value)
{
	if(!bUseClearMesh)
	{
		GetMesh()->SetMaterial(0,BaseMat);
		bUseClearMesh = true;
	}
	else
	{
		GetMesh()->SetMaterial(0,ClearMat);
		bUseClearMesh = false;
	}
}

void AChaosTankPawn::DefaultView(const FInputActionValue& Value)
{
	if(CamEnum == ECameraType::EDEFAULTCAM)
	{
		
	}
	else
	{
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(true);
		CamEnum = ECameraType::EDEFAULTCAM;
	}
}

void AChaosTankPawn::CommanderView(const FInputActionValue& Value)
{
	if(CamEnum == ECameraType::ECOMMANDERCAM)
	{
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(true);
		CamEnum = ECameraType::EDEFAULTCAM;
	}
	else
	{
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(false);
		ZoomCamera->SetActive(true);
		CamEnum = ECameraType::ECOMMANDERCAM;
	}
}

void AChaosTankPawn::GunnerView(const FInputActionValue& Value)
{
	if(CamEnum == ECameraType::EGUNNERCAM)
	{
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(true);
		CamEnum = ECameraType::EDEFAULTCAM;
	}
	else
	{
		TestPlayerCamera->SetActive(false);
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(true);
		CamEnum = ECameraType::EGUNNERCAM;
	}
}

void AChaosTankPawn::ThermalCam(const FInputActionValue& Value)
{
	
	
}

void AChaosTankPawn::OnTankHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp,Warning,TEXT("Tank Health: %d"), TankHealth);
	TankHealth -= 25.f;
}

void AChaosTankPawn::HealthCheck()
{
	//If tank is dead call detonate function and stop possession
	
	if (TankHealth <= 0){Detonate();}
	
}

void AChaosTankPawn::Detonate()
{
	const FVector TurretImpulse = {-15000.f,0.f,1000000.f};
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->BreakConstraint(TurretImpulse,GetMesh()->GetSocketLocation(TurretBone),TurretBone);
	
	if(DeathSystem)
	{
		FVector Scale = {1,1,1};
		DeathInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),DeathSystem,GetMesh()->GetSocketLocation(TurretBone),FRotator::ZeroRotator,Scale);
		DeathInstance->Activate();
	}
	GetVehicleMovement()->SetBrakeInput(1.f);
	GetVehicleMovement()->SetThrottleInput(0.f);
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle,this,&AChaosTankPawn::Die,3.f,true);
	
}

void AChaosTankPawn::Die()
{
	//functionality merged with Detonate(), just destroys the actor now
	Destroy();
}

void AChaosTankPawn::SetHitComponent_Implementation(USceneComponent* HitComponent)
{
	//UE_LOG(LogTemp,Warning,TEXT("Implementation Called"));
	if(HitComponent == EngineBlock)
	{
		EngineEnum = EENGINEBROKE;
		GetVehicleMovement()->SetThrottleInput(0.f);
		GetVehicleMovementComponent()->SetBrakeInput(1.f);
		TankHealth -= 30;
		UE_LOG(LogTemp,Warning,TEXT("Engine Damaged"));
	}
	else if(HitComponent == GunBreech)
	{
		if(BreechEnum == EBREECHDAMAGED)
		{
			BreechEnum = EBREECHBROKE;
			TankHealth -= 25.f;
			bCanShoot = false;
			UE_LOG(LogTemp,Warning,TEXT("Breech Destroyed"));
		}
		else
		{
			BreechEnum = EBREECHDAMAGED;
			TankHealth -= 20.f;
			ReloadTime = 12.f;
			UE_LOG(LogTemp,Warning,TEXT("Breech Damaged"));
			
		}
	}
	else if (HitComponent == AmmoStowage)
	{
		AmmoStowageEnum = EAMMODESTROYED;
		TankHealth = 0;
		
		UE_LOG(LogTemp,Warning,TEXT("Breech Destroyed"));
	}
	HealthCheck();
	//IDamageInterface::SetHitComponent_Implementation(HitComponent);
}

void AChaosTankPawn::Reload()
{
	//Reset can shoot;
	if(!bCanShoot){bCanShoot = true;}
	
}

void AChaosTankPawn::ReloadMG()
{
	if(MGMagazine <= 0)
	{
		MGMagazine = MGMagCapacity;
	}
	else if(MGMagazine > 0 )
	{
		//still need to add manual reload but this will simulate reloading with one bullet in the chamber
		MGMagazine = MGMagCapacity + 1;
	}
}

void AChaosTankPawn::ReloadInteriorMagazine(const FInputActionValue& Value)
{
	bool ReloadVal = Value.Get<bool>();
	
	if(ReloadVal)
	{
		if(InteriorMagazine < InteriorCapacity)
		{
			InteriorMagazine++;
			
		}
	}
	
}

void AChaosTankPawn::SetMatScalarSpeed(int Index, float Speed)
{
	//Changes the Speed Scalar Param in the Dynamic MIs to the provided speed, uses the Index param to determine which
	//track is changed(0 = Left, 1 = Right, 2 = Both)
	if(DynamicLeftTrack && DynamicRightTrack)
	{
		switch (Index)
		{
		case 0 :
			DynamicLeftTrack->SetVectorParameterValue(FName("Speed"),FVector(0.0f,Speed,0.0f));
			DynamicLeftTrack->SetVectorParameterValue(FName("Speed"),FVector(0.0f,Speed,0.0f));
			break;
		case 1:
			DynamicRightTrack->SetVectorParameterValue(FName("Speed"),FVector(0.0f,Speed,0.0f));
			break;
		case 2:
			DynamicLeftTrack->SetVectorParameterValue(FName("Speed"),FVector(0.0f,Speed,0.0f));
			DynamicRightTrack->SetVectorParameterValue(FName("Speed"),FVector(0.0f,Speed,0.0f));
			break;
		default:
			break;
		}
	}
	
}





