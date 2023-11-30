// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "DrawDebugHelpers.h"
#include "MathHelper.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Shell = CreateDefaultSubobject<UStaticMeshComponent>("Shell");
	SetRootComponent(Shell);
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
}

void ABaseProjectile::ForceLookForward()
{
	if(bForceLookForward)
	{
		SetActorRotation(UKismetMathLibrary::MakeRotFromX(GetActorForwardVector()));
	}
	
}



