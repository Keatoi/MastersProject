// Masters Project - Owen S Atkinson



#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MathHelper.h"
#include "Camera/CameraComponent.h"
#include "Math/Vector.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//=====Cam Setup======
	//Instantiating Class Components
	
	
	
	
	TestPlayerCamera = CreateDefaultSubobject<UCameraComponent>("Cam");
	TestPlayerCamera->SetupAttachment(GetMesh());
	ZoomCamera = CreateDefaultSubobject<UCameraComponent>("Commander Cam");
	ZoomCamera->SetRelativeLocationAndRotation(FVector::ZeroVector,FRotator::ZeroRotator);
	ZoomCamera->SetupAttachment(GetMesh(),"CommanderCamSocket");
	
	ZoomCamera->SetActive(false);
	GunnerCamera = CreateDefaultSubobject<UCameraComponent>("Gunner Cam");
	GunnerCamera->SetRelativeLocationAndRotation(FVector::ZeroVector,FRotator::ZeroRotator);
	GunnerCamera->SetupAttachment(GetMesh(),"GunCamSocket");
	GunnerCamera->SetActive(false);

	//Niagara spawn scene comps
	
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EngineCheck();
	TurretRingCheck();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Get Player Controller and Enhanced Input Subsystem
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	const auto eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	eiSubsystem->AddMappingContext(InputMapping, 0);
		UE_LOG(LogTemp,Warning,TEXT("EIC loaded"));
		UEnhancedInputComponent* Ei = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		Ei->BindAction(InputMove,ETriggerEvent::Triggered,this,&APlayerCharacter::Move);
	    
		Ei->BindAction(InputLook,ETriggerEvent::Triggered,this,&APlayerCharacter::Look);
		Ei->BindAction(InputFirePrimary,ETriggerEvent::Started,this,&APlayerCharacter::PrimaryFire);
	   
		Ei->BindAction(InputDefaultCam,ETriggerEvent::Started,this,&APlayerCharacter::DefaultView);
		Ei->BindAction(InputZoomCam,ETriggerEvent::Started,this,&APlayerCharacter::CommanderView);
		Ei->BindAction(InputGunnerCam,ETriggerEvent::Started,this,&APlayerCharacter::GunnerView);
	
	

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//movement code
	
	/*const FVector2d MoveValue = Value.Get<FVector2d>();
	const FRotator MoveRot(0,Controller->GetControlRotation().Yaw,0);
	if(MoveValue.Y != 0.0f && EngineEnum != EEngineStatus::EDESTROYED)//Forward/Backwards
	{
		EngineEnum = EEngineStatus::ERUNNING;
		const FVector Direction = MoveRot.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction,MoveValue.Y);
	}
	if(MoveValue.X != 0.0f )
	{
		
		AddControllerYawInput(MoveValue.X  * TankRotationSpeed);
	}*/

}



void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();
	//UE_LOG(LogTemp, Display, TEXT("look value: %f"), Value.Get<float>());
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

void APlayerCharacter::PrimaryFire(const FInputActionValue& Value)
{
	if(const bool FireValue = Value.Get<bool>())
	{
		if(FireEffectMuzzle)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle,MuzzleFlashComponent,NAME_None,FVector(0.f),FRotator(0.f),EAttachLocation::Type::KeepRelativeOffset, true);
			//NiagaraComp->SetNiagaraVariableFloat(FString("MuzzleCoefStrength"),MuzzleCoefStrength);
		}
		if(!ProjectileClass)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("No Projectile Class Loaded!!!(Primary)"));
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
				if(Projectile){FVector LaunchDirection = SpawnRotation.Vector(); Projectile->Launch(LaunchDirection);}
				
			}
		}
	}
	else
	{
		return;
	}
 
	
	
	
	
}

void APlayerCharacter::SecondaryFire(const FInputActionValue& Value)
{
	if(const bool FireValue = Value.Get<bool>())
	{
		if(!MachineGunProjectileClass)
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("No Projectile Class Loaded!!!(Secondary)"));
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



void APlayerCharacter::DefaultView(const FInputActionValue& Value)
{
	/*if(CamEnum == ECamType::EDEFAULTCAM)
	{
		
	}
	else
	{
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(true);
		CamEnum = ECamType::EDEFAULTCAM;
	}*/
}

void APlayerCharacter::CommanderView(const FInputActionValue& Value)
{
	/*if(CamEnum == ECamType::ECOMMANDERCAM)
	{
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(true);
		CamEnum = ECamType::EDEFAULTCAM;
	}
	else
	{
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(false);
		ZoomCamera->SetActive(true);
		CamEnum = ECamType::ECOMMANDERCAM;
	}*/
}

void APlayerCharacter::GunnerView(const FInputActionValue& Value)
{
	/*if(CamEnum == ECamType::EGUNNERCAM)
	{
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(false);
		TestPlayerCamera->SetActive(true);
		CamEnum = ECamType::EDEFAULTCAM;
	}
	else
	{
		TestPlayerCamera->SetActive(false);
		ZoomCamera->SetActive(false);
		GunnerCamera->SetActive(true);
		CamEnum = ECamType::EGUNNERCAM;
	}*/
}

void APlayerCharacter::EngineCheck()
{
	/*if(EngineEnum == EEngineStatus::EDESTROYED)
	{
		//SetMaxSpeed to 0.f Spawn effects (not yet made)
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else if(EngineEnum == EEngineStatus::EIDLE)
	{
		//Play idle sound, emit exhaust
	}
	else
	{
		//play running sound, emit exhaust, set max speed
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}*/
}

void APlayerCharacter::TurretRingCheck()
{
	
}

