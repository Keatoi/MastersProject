// Masters Project - Owen S Atkinson


#include "ChaosTankPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MathHelper.h"
#include "Camera/CameraComponent.h"
#include "Math/Vector.h"

AChaosTankPawn::AChaosTankPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//=====Cam Setup======
	TestPlayerCamera = CreateDefaultSubobject<UCameraComponent>("Cam");
	TestPlayerCamera->SetupAttachment(GetMesh());
	ZoomCamera = CreateDefaultSubobject<UCameraComponent>("Commander Cam");
	ZoomCamera->SetupAttachment(GetMesh());
	ZoomCamera->SetActive(false);
	GunnerCamera = CreateDefaultSubobject<UCameraComponent>("Gunner Cam");
	GunnerCamera->SetupAttachment(GetMesh(),"GunCamSocket");
	GunnerCamera->SetActive(false);
}

void AChaosTankPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AChaosTankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChaosTankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Get Player Controller and Enhanced Input Subsystem
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	auto eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	eiSubsystem->AddMappingContext(InputMapping, 0);
	UE_LOG(LogTemp,Warning,TEXT("EIC loaded"));
	UEnhancedInputComponent* Ei = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Ei->BindAction(InputMove,ETriggerEvent::Triggered,this,&AChaosTankPawn::Move);
	Ei->BindAction(InputLook,ETriggerEvent::Triggered,this,&AChaosTankPawn::Look);
	Ei->BindAction(InputFirePrimary,ETriggerEvent::Started,this,&AChaosTankPawn::PrimaryFire);
	Ei->BindAction(InputCameraSwap,ETriggerEvent::Started,this,&AChaosTankPawn::CameraSwap);
	Ei->BindAction(InputDefaultCam,ETriggerEvent::Started,this,&AChaosTankPawn::DefaultView);
	Ei->BindAction(InputZoomCam,ETriggerEvent::Started,this,&AChaosTankPawn::CommanderView);
	Ei->BindAction(InputGunnerCam,ETriggerEvent::Started,this,&AChaosTankPawn::GunnerView);
}

void AChaosTankPawn::Move(const FInputActionValue& Value)
{
	//movement code
	//UE_LOG(LogTemp, Display, TEXT("move value: %f"), Value.Get<float>());
	const FVector2d MoveValue = Value.Get<FVector2d>();
	const FRotator MoveRot(0,Controller->GetControlRotation().Yaw,0);
	if(MoveValue.Y != 0.0f)//Forward/Backwards
		{
		const FVector Direction = MoveRot.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction,MoveValue.Y);
		}
	if(MoveValue.X != 0.0f)
	{
		
		AddControllerYawInput(MoveValue.X);
	}
}

void AChaosTankPawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Display, TEXT("look value: %f"), Value.Get<float>());
	if (LookValue.X != 0.f)
	{
		TurretTraverse += LookValue.X * 10.f;
	}
 
	if (LookValue.Y != 0.f)
	{
		TurretElevation += LookValue.Y * 10.f;
		TurretElevation = FMath::Clamp(TurretElevation,-10.f,10.f);
	}
}

void AChaosTankPawn::PrimaryFire(const FInputActionValue& Value)
{
	if(const bool FireValue = Value.Get<bool>())
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
				FActorSpawnParameters SParams;
				SParams.Owner = this;
				SParams.Instigator = GetInstigator();
				//Spawn Projectile At location of the Main Barrels Socket
				FVector SpawnLocation = GetMesh()->GetSocketLocation("Main_CaliberSocket");
				FRotator SpawnRotation = GetMesh()->GetSocketRotation("Main_CaliberSocket");
				ABaseProjectile* Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass,SpawnLocation,SpawnRotation,SParams);
				
			}
		}
	}
	else
	{
		return;
	}
}

void AChaosTankPawn::SecondaryFire(const FInputActionValue& Value)
{
	if(const bool FireValue = Value.Get<bool>())
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
				
			}
		}
	}
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
