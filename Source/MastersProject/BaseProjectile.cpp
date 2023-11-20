// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "DrawDebugHelpers.h"
#include "MathHelper.h"

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
	FVector LastPosition = GetActorLocation();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bHasHit)
	{
		//do something if it has hit 
		
	}

}



