// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"

#include "ArmourActor.h"
#include "ArmourInterface.h"
#include "DrawDebugHelpers.h"
#include "MathHelper.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(rootComp);
	Shell = CreateDefaultSubobject<UStaticMeshComponent>("Shell");
	Shell->SetupAttachment(rootComp);
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->InitSphereRadius(15.f);
	Sphere->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	InitialForce = Force;
	SetInitialPosition();
	SetInitialVelocity();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaT = DeltaTime;
	Move();
	ApplyGravity();
	ForceLossFunc();
	ForceLookForward();

}

void ABaseProjectile::SetInitialPosition()
{
	//Call in BeginPlay to get Location at spawn
	InitialLocation = GetActorLocation();
}

void ABaseProjectile::SetInitialVelocity()
{
	//Push actor forward by a speed dependant on the force applied to it
	Velocity = GetActorForwardVector() * Force;
}

void ABaseProjectile::Move()
{
	//Offset actor each tick, BSweep = true to check for blocking collisions
	AddActorWorldOffset(Velocity * Force,true);
	
	CheckCollision();
	
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
	FHitResult HitResult;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation  = StartLocation + Velocity * DeltaT;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,EndLocation,TraceChannelProperty,QueryParams);
	if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Actor: %s"), *HitResult.GetActor()->GetName());
		FVector NormalFWD = GetActorForwardVector().GetSafeNormal();
		float Angle = MathHelper::CalculateAngleofImpact(HitResult.Normal,NormalFWD);
		UE_LOG(LogTemp, Log, TEXT("Angle: %f"), Angle);
		ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(HitResult.Location,FRotator());
		
		if(HitResult.GetActor()->IsA<AArmourActor>())
		{
			UE_LOG(LogTemp,Log,TEXT("Actor is an armour panel"));
			AArmourActor* HitArmour = Cast<AArmourActor>(HitResult.GetActor());
			float Platethickness  = HitArmour->Thickness;//Assumes homogenous steel, will make adjustments for composite later
			float EffectiveThickness = MathHelper::CalculateRelativeArmourThickness(Platethickness,Angle);// Get actual amount of armour we need the round to pen
			if (EffectiveThickness < PenetrationAmount)
			{
				//projectile penetrates
				bPenetrated = true;
			}
			
		}
		switch(HitResult.PhysMaterial->SurfaceType)
		{
		case(EPhysicalSurface::SurfaceType1):
			{
				//If metal and penetration successful
					if(bPenetrated == true)
					{
						Decal->SetDecalMaterial(PenDecal);
					}
					else
					{
						Decal->SetDecalMaterial(NonPenDecal);
					}
						
				break;
			}
		case(EPhysicalSurface::SurfaceType2):
			{
				//Walls
				
				Decal->SetDecalMaterial(GenericDecal);	
				break;
			}


				
			
		default:;break;
		}
		Decal->SetLifeSpan(10.0f);
		Decal->GetDecal()->DecalSize = DecalSize;
		
		
		
	}
	
}



void ABaseProjectile::NullVelocity()
{
	Velocity = FVector(0.f,0.f,0.f);
}

void ABaseProjectile::ProxCheck()
{
	//checks if a actor with a certain surface type is in radius. Requires a Minimum Range Fuse and radius and a projectile with explosive filler
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
			//Explosion code stuff here
		}
			
	}
}

float ABaseProjectile::DistanceTravelled()
{
	FVector DistanceVector =  GetActorLocation() - InitialLocation;
	return DistanceVector.Length();
}



