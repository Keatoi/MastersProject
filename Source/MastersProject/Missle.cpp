// Masters Project - Owen S Atkinson


#include "Missle.h"

#include "MathHelper.h"
#include "Engine/DamageEvents.h"
#include "BombComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMissle::AMissle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Actor Component Initialisation
	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = SM;
	SM->SetSimulatePhysics(true);
	SM->SetEnableGravity(false);//Gravity disabled since the missile has no fuel system so will be eternally propelled
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(SM);
	HEComponent = CreateDefaultSubobject<UBombComponent>(TEXT("BombComponent"));
	
	
}

void AMissle::Launch(FVector Direction)
{
	SM->AddImpulse(this->GetActorForwardVector() * Speed);
}

void AMissle::SetTarget()
{
	
}

void AMissle::ProximityCheck()
{
	//checks if a vehicle actor  is in radius. Requires a Minimum Range Fuse and radius and a projectile with explosive filler.
	if(MathHelper::GetDistance(StartLocation,CurrLocation) > MinRange)
	{
		UE_LOG(LogTemp, Log, TEXT("IN RANGE"));
		// if the minimum range is surpassed check for collisions
		TArray<FHitResult> Hits;
		//start and end
		FVector StartSweep = GetActorLocation();
		FVector EndSweep = GetActorLocation();
		FCollisionShape CollisionTrace = FCollisionShape::MakeSphere(DetectRange);
		if(bDebug){DrawDebugSphere(GetWorld(),GetActorLocation(),DetectRange,50,FColor::Red,true);}
		bool bisHit = GetWorld()->SweepMultiByChannel(Hits,StartSweep,EndSweep,FQuat::Identity,ECC_Vehicle,CollisionTrace);
		if(bisHit)
		{
			//Explosion code stuff here- see onHit.
			if(ExplosiveFiller > 1.f && HEComponent->IsValidLowLevelFast())
			{
				UE_LOG(LogTemp, Log, TEXT("Exploding(PROX)"));
				HEComponent->CreateFireball(GetActorLocation(),ExplosiveFiller,500.f);
			}
		}
			
	}
}

void AMissle::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


void AMissle::MissleRot()
{
	//Find look at rotation, get Rinterp and set new rotation
	FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLoc);
	FRotator NewRot =  FMath::RInterpTo(GetActorRotation(),TargetRot,GetWorld()->GetDeltaSeconds(),InterpSpeed);
	SetActorRotation(NewRot);
}

void AMissle::Detonate()
{
	//Clear Timers
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	//TODO: Damage Func
	
	//TODO: Add BombComp Function

	//Kill Self 
	Destroy();
}

void AMissle::SetTargetLocation_Implementation(FVector TargetLocation)
{
	TargetLoc = TargetLocation;
	//IGuidedInterface::SetTargetLocation_Implementation(TargetLocation);
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();
	//Call delegate
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this,&AMissle::OnOverlapBegin);
	//Set Start Location
	StartLocation = GetActorLocation();
	if(bSelfDestruct)
	{
		GetWorld()->GetTimerManager().SetTimer(MissleHandle,FuseTimer,false);
	}
}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Update Acceleration
	Accel += DeltaTime * AccelRate;
	//Update missile speed
	Speed += Accel * DeltaTime;
	//Update Pos
	CurrLocation = GetActorLocation();
	//Move the missile
	SM->AddImpulse(this->GetActorForwardVector() * Speed);
	//Rotate to face target Location
	MissleRot();
	//Proximity check
	ProximityCheck();
		
}

