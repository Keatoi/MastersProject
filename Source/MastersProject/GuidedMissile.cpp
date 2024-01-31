// Masters Project - Owen S Atkinson


#include "GuidedMissile.h"

#include "GameFramework/ProjectileMovementComponent.h"

void AGuidedMissile::Launch(FVector MoveDirection)
{
	if(!bHasTarget)
	{
		ProjectileMovementComponent->Velocity = GetActorUpVector() * MissileLaunchDistance;
	}
}
