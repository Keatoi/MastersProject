// Masters Project - Owen S Atkinson


#include "PlayerDrone.h"

#include "Camera/CameraComponent.h"

// Sets default values
APlayerDrone::APlayerDrone()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DroneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Drone Mesh"));
	check(DroneMesh);
	FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	check(FollowCam);
	FollowCam->SetupAttachment(DroneMesh);
}

// Called when the game starts or when spawned
void APlayerDrone::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

// Called every frame
void APlayerDrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Update Acceleration
	Accel += DeltaTime * AccelRate;
	//Update missile speed
	Speed += Accel * DeltaTime;
	//Update Pos
	CurrLocation = GetActorLocation();
	//Move the missile
	DroneMesh->AddForce(this->GetActorForwardVector() * Speed);

}

// Called to bind functionality to input
void APlayerDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

