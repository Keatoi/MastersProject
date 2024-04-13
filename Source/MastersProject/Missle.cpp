// Masters Project - Owen S Atkinson


#include "Missle.h"

#include "MathHelper.h"
#include "Engine/DamageEvents.h"
#include "BombComponent.h"

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
	
	
}

void AMissle::Launch(FVector Direction)
{
}

void AMissle::SetTarget(AActor* Target)
{
}

void AMissle::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AMissle::MissleRot(float RotSpeed)
{
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();
	//Add Delegate to BeginOverlap
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this,&AMissle::OnOverlapBegin);
	//Set Start Location
	StartLocation = GetActorLocation();
	
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
	//Rotate to face target Actor, assuming it is set
	if(TargetActor){MissleRot(InterpSpeed);}
		
}

