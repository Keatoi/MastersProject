// Masters Project - Owen S Atkinson


#include "GuidedMissile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AGuidedMissile::AGuidedMissile()
{
	ProjectileMovementComponent->bIsHomingProjectile = true;
}

void AGuidedMissile::Launch(FVector MoveDirection)
{
	if(!bHasTarget)
	{
		ProjectileMovementComponent->Velocity = GetActorUpVector() * MissileLaunchDistance;
	}
	
}
