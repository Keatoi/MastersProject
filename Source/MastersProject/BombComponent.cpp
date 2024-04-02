// Masters Project - Owen S Atkinson
#include "BombComponent.h"

#include "NiagaraComponent.h"
#include  "ShrapnelComponent.h"

#include "Components/SphereComponent.h"

#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UBombComponent::UBombComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	FireBallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Fireball Collision"));
	FireBallCollision->InitSphereRadius(125.f);
	
	ShrapnelComponent = CreateDefaultSubobject<UShrapnelComponent>(TEXT("Shrapnel Component"));
	// ...
}


// Called when the game starts
void UBombComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBombComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBombComponent::CreateFireball(float range,float strength,FVector Location)
{
	UE_LOG(LogTemp,Warning,TEXT("Creating Fireball"));
	TArray<AActor*> OverlapArray;
	FireBallCollision->SetSphereRadius(range);
	FireBallCollision->GetOverlappingActors(OverlapArray);
	//Use the Emmitter for the center of the fireball and the NS for the sparks/smoke
	if(FireballEmitter)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),FireballEmitter,Location);
	}
	if(BombSystem)
	{
		FVector Scale = {1,1,1};
		BombInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),BombSystem,Location,FRotator::ZeroRotator,Scale);
		BombInstance->Activate();
	}
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NSFireball,Location);
	for(AActor* Actors : OverlapArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlappingActors: %d"), OverlapArray.Num());
		UStaticMeshComponent* GetStaticMesh = Cast<UStaticMeshComponent>((Actors)->GetRootComponent());
		
		if(GetStaticMesh)
		{
			GetStaticMesh->AddRadialImpulse(Location,range,strength,RIF_Linear,true);
			
		}
		else
		{
			USkeletalMeshComponent* GetSKM = Cast<USkeletalMeshComponent>(Actors->GetRootComponent());
			if(GetSKM)
			{
				GetSKM->AddRadialImpulse(Location,range,strength,RIF_Linear,true);
			}
		}
		Actors->TakeDamage(strength,FDamageEvent(),GetOwner()->GetInstigatorController(),GetOwner());
	}
	
	//Originally the bomb code had the sphere expand like an actual fireball before I realised how stupid it would be for optimisation purposes
	/*while(FireBallCollision->GetUnscaledSphereRadius() <= range)
	{
		float newRadius = FireBallCollision->GetUnscaledSphereRadius() + RadiusIncrement;
		FireBallCollision->SetSphereRadius(newRadius);
	}*/
	
}

