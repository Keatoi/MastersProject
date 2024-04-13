// Masters Project - Owen S Atkinson


#include "GuidedMissile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AGuidedMissile::AGuidedMissile()
{
	
}

void AGuidedMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGuidedMissile::Launch(FVector MoveDirection)
{
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed;
	
}
