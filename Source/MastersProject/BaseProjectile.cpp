// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"

#include "ArmourActor.h"
#include "ArmourInterface.h"
#include "DamageInterface.h"
#include "DrawDebugHelpers.h"
#include "MathHelper.h"
#include "Engine/DamageEvents.h"
#include "BombComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(!RootComponent){RootComponent = CreateDefaultSubobject<USceneComponent>("Root");}
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->InitSphereRadius(15.f);
	
	RootComponent = Sphere;
	Shell = CreateDefaultSubobject<UStaticMeshComponent>("Shell");
	Shell->SetupAttachment(Sphere);
	BombComponent = CreateDefaultSubobject<UBombComponent>(TEXT("Bomb Component"));
	Speed = 3000.f;//Used if the simple custom projectile movement is used
	if(!ProjectileMovementComponent)
	{
		//Projectile Move comp setup
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(Sphere);
		if(bSloMoMode){ProjectileMovementComponent->InitialSpeed = 30.0f;}
		else{ProjectileMovementComponent->InitialSpeed = 3000.0f;}
		
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	bDoOnce = true;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	InitialForce = Force;
	SetInitialPosition();
	SetInitialVelocity();
	Sphere->OnComponentHit.AddDynamic(this,&ABaseProjectile::OnHit);
	
}

void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaT = DeltaTime;
	//CheckCollision(); DEPRECATED DUE TO UNRELIABILITY
	ForceLookForward();
	//usage of custom simplified movement methods (Gravity is separate so it can be used with Projectile Move Comp in projectiles that don't have physics enabled like this one)
	if(bApplyGravity){ApplyGravity();}
	if(bUseCustomMove){Move();}
}

void ABaseProjectile::SetInitialPosition()
{
	//Call in BeginPlay to get Location at spawn
	InitialLocation = GetActorLocation();
}

void ABaseProjectile::SetInitialVelocity()
{
	//Push actor forward by a speed dependant on the force applied to it
	Velocity = GetActorForwardVector() * Speed;
}

void ABaseProjectile::Move()
{
	//Custom Movement
	if(bUseCustomMove)
	{
		//Offset actor each tick, BSweep = true to check for blocking collisions
		AddActorLocalOffset(Velocity,true);
	}
}

void ABaseProjectile::Launch(FVector MoveDirection)
{
	//Set ProjMoveComp Velocity
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed;
	
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Actor: %s"), *Hit.GetActor()->GetName());
	//Calculate the angle the projectile hits another actor and output to log
	FVector NormalFWD = GetActorForwardVector().GetSafeNormal();
	float Angle = 180.f - MathHelper::CalculateAngleofImpact(Hit.Normal,NormalFWD);
	UE_LOG(LogTemp, Log, TEXT("Angle: %f"), Angle);
	//check if hit object uses a physical material(this is what the tank armour relies upon)
	if(Hit.PhysMaterial->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Log, TEXT("Is Valid"));
		//Uses the density of the physical material as the armour thickness (in mm)
		float HitArmourThickness = Hit.PhysMaterial->Density;
		UE_LOG(LogTemp, Log, TEXT("Armour: %f"), HitArmourThickness);
		//The projectiles base penetration
		UE_LOG(LogTemp, Log, TEXT("Penetration: %f"), PenetrationAmount);
		//Calculate the effective amount of armour using the actual armour thickness and hit angle
		float RelativeArmourThickness = MathHelper::CalculateRelativeArmourThickness(HitArmourThickness,Angle);
		UE_LOG(LogTemp, Log, TEXT("Relative Armour: %f"), RelativeArmourThickness);
		//If the projectile can penetrate proceed with damage implementation
		if(PenetrationAmount > RelativeArmourThickness)
		{
			float Damage = PenetrationAmount - RelativeArmourThickness;
			OtherActor->TakeDamage(Damage,FDamageEvent(),GetInstigatorController(),this);
			float PostPenAmount = PenetrationAmount - RelativeArmourThickness;//Change Penetration amount
			UE_LOG(LogTemp, Log, TEXT("Remaining Penetration: %f"), PostPenAmount);
			bPenetrated = true;
		}
		else
		{
			bPenetrated = false;
		}
	}
	//if projectile penetrated and the hit actor implements the damage interface, for each hit component call the interface func,
	//which adds the component to an array accessible by the Hit Actor allowing for mechanics such as component based damage models
	if(bPenetrated && Hit.GetActor()->GetClass()->ImplementsInterface(UDamageInterface::StaticClass()))
	{
		TArray<FHitResult> InteriorHit;
		FVector StartLocation = Sphere->GetComponentLocation();
		FVector ForwardVector = GetActorForwardVector();
		FVector EndLocation  = (ForwardVector * 1000.f) + StartLocation;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		GetWorld()->LineTraceMultiByProfile(InteriorHit,StartLocation,EndLocation,FName("Interior"),QueryParams);
		for(int i = 0; i <= InteriorHit.Num() - 1; i++)
		{
			
			FString HitCompName = InteriorHit[i].GetComponent()->GetName();
			UE_LOG(LogTemp, Log, TEXT("Hit Interior Comp: %s"), *HitCompName);
			IDamageInterface::Execute_SetHitComponent(Hit.GetActor(),InteriorHit[i].GetComponent());
			
		}
		
	}
	//check if bomb component is valid and there is explosive filler in the projectile
	//if so detonate
	if(!BombComponent->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Log, TEXT("Bomb not valid"));
	}
	if(ExplosiveFiller > 1.f && BombComponent->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Log, TEXT("Exploding"));
		BombComponent->CreateFireball(100.f,ExplosiveFiller,GetActorLocation());
		
	
	}
	
	
	
	//destroy actor
	Destroy();
}

void ABaseProjectile::ApplyGravity()
{
	//Lower the Velocity by the grav value over time
	if(bApplyGravity)
	{
		Velocity += FVector(0.f,0.f,-(Gravity * DeltaT));
	}
}

void ABaseProjectile::ForceLossFunc()
{
	
	Force -= ForceLoss * DeltaT;
	if(Force < 0.0f){Destroy();}
	Velocity = Velocity * Force;
	
}

void ABaseProjectile::ForceLookForward()
{
	if(bForceLookForward)
	{
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(GetActorForwardVector()));
	}
	
}

void ABaseProjectile::CheckCollision()
{
	/*FHitResult HitResult;
	FVector StartLocation = Sphere->GetComponentLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndLocation  = (ForwardVector * 500.f) + StartLocation;
	const FVector DirectionUnitVector = UKismetMathLibrary::GetDirectionUnitVector(StartLocation,EndLocation);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,EndLocation,TraceChannelProperty,QueryParams);
	if(bMarkPath){UKismetSystemLibrary::DrawDebugLine(GetWorld(),StartLocation,HitResult.Location,FColor::Red,5.f);}
	if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
	{
		
		if(Angle > RicochetAngle)
		{
			StartLocation = HitResult.Location;
			EndLocation = (1000.f * FMath::GetReflectionVector(DirectionUnitVector,HitResult.Normal)) + StartLocation;
			if(bMarkPath) {
				FHitResult RicochetHit;
				UKismetSystemLibrary::DrawDebugLine(GetWorld(),StartLocation,RicochetHit.Location,FColor::Green,5.f);
			}
			FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(),EndLocation);
			FRotator InterpedRot = FMath::RInterpTo(Sphere->GetComponentRotation(),NewRot,DeltaT,1.f);
			SetActorRotation(InterpedRot);
		}
		if(HitResult.GetActor()->IsA<AArmourActor>())
		{
			UE_LOG(LogTemp,Log,TEXT("Actor is an armour panel"));
			AArmourActor* HitArmour = Cast<AArmourActor>(HitResult.GetActor());
			float Platethickness  = HitArmour->Thickness;//Assumes homogenous steel, will make adjustments for composite later
			float EffectiveThickness = MathHelper::CalculateRelativeArmourThickness(Platethickness,Angle);// Get actual amount of armour we need the round to pen
			UE_LOG(LogTemp, Log, TEXT("Thickness: %f"), EffectiveThickness);
			if (EffectiveThickness < PenetrationAmount)
			{
				//projectile penetrates
				bPenetrated = true;
				UE_LOG(LogTemp,Warning,TEXT("Penetrated"));
			}
			else
			{
				bPenetrated = false;
				UE_LOG(LogTemp,Warning,TEXT("NOT Penetrated"));
			}
			
		}*/
		/*if (bDoOnce)
        {
            FRotator RandomDecalRotation=HitResult.ImpactNormal.Rotation();  
            		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f); 
            		UGameplayStatics::SpawnDecalAttached(GenericDecal,DecalSize,HitResult.GetComponent(),NAME_None,HitResult.ImpactPoint,RandomDecalRotation,EAttachLocation::KeepWorldPosition,DecalLifespan);
            bDoOnce = false;
        }*/
		
		
		
		
	
	
}



void ABaseProjectile::NullVelocity()
{
	Velocity = FVector(0.f,0.f,0.f);
}

void ABaseProjectile::ProxCheck()
{
	//checks if a vehicle actor  is in radius. Requires a Minimum Range Fuse and radius and a projectile with explosive filler.
	if(DistanceTravelled() > MinRange)
	{
		// if the minimum range is surpassed check for collisions
		TArray<FHitResult> Hits;
		//start and end
		FVector StartSweep = GetActorLocation();
		FVector EndSweep = GetActorLocation();
		FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(DetectRadii);
		if(bPenDebug){DrawDebugSphere(GetWorld(),GetActorLocation(),DetectRadii,50,FColor::Red,true);}
		bool isHit = GetWorld()->SweepMultiByChannel(Hits,StartSweep,EndSweep,FQuat::Identity,ECC_Vehicle,CollisionSphere);
		if(isHit)
		{
			//Explosion code stuff here- see onHit.
			if(ExplosiveFiller > 1.f && BombComponent->IsValidLowLevelFast())
			{
				UE_LOG(LogTemp, Log, TEXT("Exploding(PROX)"));
				BombComponent->CreateFireball(100.f,ExplosiveFiller,GetActorLocation());
			}
		}
			
	}
}

float ABaseProjectile::DistanceTravelled()
{
	//gets the distance travelled by the actor since spawn, used for Proximity check
	FVector DistanceVector =  GetActorLocation() - InitialLocation;
	return DistanceVector.Length();
}



