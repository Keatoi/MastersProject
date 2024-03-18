// Masters Project - Owen S Atkinson


#include "ChaosTankPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MathHelper.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Channels/MovieSceneDoubleChannel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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
	/*Timeline setup 
	 */
	TimeLineUpdateEvent.BindDynamic(this, &AChaosTankPawn::TurretDetonationImpulse);
	TimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("Detonation TimeLine"));
	bCanShoot = true;//Ensure we can shoot at spawn
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
}

void AChaosTankPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Clear Timers if actor removed from level (i.e destroyed or despawned)
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
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
	if(GetVehicleMovementComponent()->GetThrottleInput() == 0.f && SB_Engine_Idle){UGameplayStatics::PlaySoundAtLocation(GetWorld(),SB_Engine_Idle,EngineBlock->GetComponentLocation());}
	else if(SB_Engine){UGameplayStatics::PlaySoundAtLocation(GetWorld(),SB_Engine_Idle,EngineBlock->GetComponentLocation());}
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
	FVector GunFWD = GetMesh()->GetSocketRotation("Main_CaliberSocket").Vector() * 10000.f;
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
	//UE_LOG(LogTemp, Display, TEXT("move value: %f"), Value.Get<float>());
	const FVector2d MoveValue = Value.Get<FVector2d>();
	if(EngineEnum == EENGINEBROKE)
	{
		MoveCancelled(Value);
	}
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
	SetMatScalarSpeed(2,0.f);
}

void AChaosTankPawn::MoveCancelled(const FInputActionValue& Value)
{
	GetVehicleMovement()->SetYawInput(0.f);
	GetVehicleMovement()->SetThrottleInput(0.f);
	SetMatScalarSpeed(2,0.f);
}

void AChaosTankPawn::TurnTriggered(const FInputActionValue& Value)
{
	if(bStopTurn)
	{
		TurnRate = UKismetMathLibrary::Lerp(TurnRate,0.f,0.1f);
		GetVehicleMovement()->SetYawInput(TurnRate);
	}
	else
	{
		TurnRate = Value.Get<float>();
		if(TurnRate > 0.f){SetMatScalarSpeed(1,TurnRate);}
		else if(TurnRate < 0.f){SetMatScalarSpeed(0,-TurnRate);}
		GetVehicleMovement()->SetYawInput(TurnRate);
	}
	
}

void AChaosTankPawn::TurnStarted(const FInputActionValue& Value)
{
	//Needs a tiny bit of throttle to turn, dont want to add to much in case the tank is moving
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
	const FVector2D LookValue = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Display, TEXT("look value: %f"), Value.Get<float>());
	if (LookValue.X != 0.f)
	{
		TurretTraverse += LookValue.X * 5.f;
		AddControllerYawInput(LookValue.X * 0.5);
	}
 
	if (LookValue.Y != 0.f)
	{
		TurretElevation += LookValue.Y * 10.f;
		TurretElevation = FMath::Clamp(TurretElevation,-6.f,14.f);
		if(bFreeLookEnabled)
		{
			AddControllerPitchInput(LookValue.Y * 0.5);
		}
		
	}
}

void AChaosTankPawn::PrimaryFire(const FInputActionValue& Value)
{
	if(const bool FireValue = Value.Get<bool>() &&(BreechEnum != EBREECHBROKE || CannonEnum != ECANNONBROKE) && bCanShoot && AmmoReserve > 0.f)//If mouse pressed and there is no gun damage and we can shoot/arent reloading/aren't out of ammo
	{
		if(!ProjectileClass)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("No Projectile Class Loaded!!!"));
		}
		else
		{
			
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Firing!!!"));
			//Spawn Parameters
			if(UWorld* World = GetWorld())
			{
				//Remove Ammo from two-tier Magazines
				AmmoReserve--;
				if(InteriorMagazine > 0) InteriorMagazine--;//Ensure Interior Magazine does not go into the negatives
				//Spawn Parameters
				FActorSpawnParameters SParams;
				SParams.Owner = this;
				SParams.Instigator = GetInstigator();
				//Spawn Projectile At location of the Main Barrels Socket
				FVector SpawnLocation = GetMesh()->GetSocketLocation("Main_CaliberSocket");
				FRotator SpawnRotation = GetMesh()->GetSocketRotation("Main_CaliberSocket");
				ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass,SpawnLocation,SpawnRotation,SParams);//Spawn Projectile
				GetMesh()->AddImpulse(FVector(-150.f,0.f,0.f),FName(NAME_None),true);//Add Impulse to simulate recoil
				
				bCanShoot = false;//Disable firing until reloaded
				if(InteriorMagazine > 0.f)ReloadTime = 3.f; else ReloadTime = 5.f;//Set ReloadTime based on Two-tier magazine ammo
				World->GetTimerManager().SetTimer(ReloadTimerHandle,this,&AChaosTankPawn::Reload,ReloadTime,false);//Start reload sequence
				if(SB_MainGun){UGameplayStatics::PlaySoundAtLocation(World,SB_MainGun,SpawnLocation,SpawnRotation);}//Play Fire Sound if valid
				if (NSFireMuzzle)
				{
					//NCMuzzle = UNiagaraFunctionLibrary::SpawnSystemAttached(NSFireMuzzle,MuzzleFlashComponent,NAME_None,MuzzleFlashComponent->GetComponentLocation(),MuzzleFlashComponent->GetComponentRotation(),EAttachLocation::SnapToTarget,true,true);
				}
			}
		}
	}
	else
	{
		return;
	}
}

void AChaosTankPawn::SecondaryFireStart(const FInputActionValue& Value)
{
	if(const bool FireValue = Value.Get<bool>())
	{
		if(MGMagazine > 0)
		{
			//If we have bullets left in magazine start the MGTimer
			GetWorld()->GetTimerManager().SetTimer(MGFireRateHandle,this,&AChaosTankPawn::SecondaryFire,0.1f,true);
		}
		
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

void AChaosTankPawn::CameraSwap(const FInputActionValue& Value)
{
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

void AChaosTankPawn::OnTankHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp,Warning,TEXT("Tank Health: %d"), TankHealth);
	TankHealth -= 50;
}

void AChaosTankPawn::HealthCheck()
{
	//If tank is dead call detonate function and stop possession
	
	if (TankHealth <= 0){Detonate();}
	UnPossessed();
}

void AChaosTankPawn::Detonate()
{
	const FVector TurretImpulse = {-15000.f,0.f,1000000.f};
	GetMesh()->BreakConstraint(TurretImpulse,GetMesh()->GetSocketLocation(TurretBone),TurretBone);
	float FinishTime = 5.f;
	TimeLine->SetPlayRate(1/FinishTime);
	
	TimeLine->SetTimelinePostUpdateFunc(TimeLineUpdateEvent);
	TimeLine->PlayFromStart();
	
	
}

void AChaosTankPawn::TurretDetonationImpulse()
{
	//const FVector ImpulsetoAdd = {0,0.f,-100000.0f};
	//GetMesh()->AddImpulseAtLocation(ImpulsetoAdd,GetMesh()->GetSocketLocation(TurretBone),TurretBone);
	UE_LOG(LogTemp,Warning,TEXT("Turret Detonation  Called"));
}

void AChaosTankPawn::SetHitComponent_Implementation(USceneComponent* HitComponent)
{
	UE_LOG(LogTemp,Warning,TEXT("Implementation Called"));
	if(HitComponent == EngineBlock)
	{
		EngineEnum = EENGINEBROKE;
		TankHealth -= 30;
		UE_LOG(LogTemp,Warning,TEXT("Engine Damaged"));
		
	}
	else if(HitComponent == GunBreech)
	{
		if(BreechEnum == EBREECHDAMAGED)
		{
			BreechEnum = EBREECHBROKE;
			TankHealth -= 40;
			UE_LOG(LogTemp,Warning,TEXT("Breech Destroyed"));
		}
		else
		{
			BreechEnum = EBREECHDAMAGED;
			TankHealth -= 20;
			UE_LOG(LogTemp,Warning,TEXT("Breech Damaged"));
			
		}
	}
	else if (HitComponent == AmmoStowage)
	{
		AmmoStowageEnum = EAMMODESTROYED;
		TankHealth = 0;
		Detonate();
		UE_LOG(LogTemp,Warning,TEXT("Breech Destroyed"));
	}
	
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
			DynamicLeftTrack->SetScalarParameterValue(FName("Speed"),Speed);
			break;
		case 1:
			DynamicRightTrack->SetScalarParameterValue(FName("Speed"),Speed);
			break;
		case 2:
			DynamicLeftTrack->SetScalarParameterValue(FName("Speed"),Speed);
			DynamicRightTrack->SetScalarParameterValue(FName("Speed"),Speed);
			break;
		default:
			break;
		}
	}
	
}



