// Masters Project - Owen S Atkinson


#include "AI_ChaosTank.h"

#include "Camera/CameraComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAI_ChaosTank::AAI_ChaosTank()
{
	TestPlayerCamera->SetActive(false);
	GunnerCamera->SetActive(false);
	ZoomCamera->SetActive(false);
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Tank Vision"));
	PawnSense->SetPeripheralVisionAngle(125.f);
	
	PawnSense->SensingInterval = .25f;//Sense every 0.25 seconds
	GetVehicleMovement()->SetThrottleInput(1.f);
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Path"));
	//Spline->SetupAttachment(GetMesh());
	Tags.Add(Team);
}

void AAI_ChaosTank::BeginPlay()
{
	Super::BeginPlay();
	PawnSense->OnSeePawn.AddDynamic(this,&AAI_ChaosTank::OnSeePawn);
	PawnSense->OnHearNoise.AddDynamic(this,&AAI_ChaosTank::OnHearNoise);
	GetVehicleMovementComponent()->SetThrottleInput(1.f);
	//Set Target Location now, if left to default at origin the spline can get stuck under the mountains at the centre of the map
	TargetLoc = GetActorForwardVector() * 1000.f;
	if(Team == "Blue"){EnemyTeam = "Red";}
	else{EnemyTeam == "Blue";}
}

void AAI_ChaosTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Spline->ClearSplinePoints();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this,0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,GetActorLocation(),TargetLoc);
	TArray<FVector> Paths  = NavPath->PathPoints;
	for (auto Path : Paths)
	{
		Spline->AddSplinePoint(Path,ESplineCoordinateSpace::World,true);
		
	}
	GetVehicleMovementComponent()->SetYawInput(Pathfinding());
}

UBehaviorTree* AAI_ChaosTank::GetBehaviourTree() const
{
	return Tree;
}

void AAI_ChaosTank::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);    
	FString message = TEXT("Heard Actor ") + OtherActor->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

void AAI_ChaosTank::OnSeePawn(APawn* OtherPawn)
{
	FString message = TEXT("Saw Actor ") + OtherPawn->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
	if(OtherPawn->ActorHasTag(EnemyTeam))
	{
		EnemyLoc = OtherPawn->GetActorLocation();
		bEnemySpotted = true;
		UE_LOG(LogTemp,Warning,TEXT("Enemy Location: %s"),*EnemyLoc.ToString())
		GetWorld()->GetTimerManager().SetTimer(AimDelayHandle,this,&AAI_ChaosTank::Attack,1.f,false);
	}
	else
	{
		bEnemySpotted = false;
	}
	//print enemyspotted result
	UE_LOG(LogTemp,Warning,TEXT("bEnemySpotted: %s"), bEnemySpotted ? TEXT("True"):TEXT("False"));
}

void AAI_ChaosTank::SetThrottle(float Throttle)
{
	GetVehicleMovement()->SetThrottleInput(Throttle);
}

void AAI_ChaosTank::SetBrake(float Brake)
{
	GetVehicleMovement()->SetBrakeInput(Brake);
}

float AAI_ChaosTank::Pathfinding()
{
	FVector ClosestTangent = Spline->FindTangentClosestToWorldLocation(GetActorLocation(),ESplineCoordinateSpace::World);
	FVector TangentNormal = ClosestTangent.GetSafeNormal(0.001) * SplineDetection;
	FVector ClosestLocation = Spline->FindLocationClosestToWorldLocation((GetActorLocation()+TangentNormal),ESplineCoordinateSpace::World);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),ClosestLocation);
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(),LookAtRotation);
	//return a clamped yaw value to use in turning
	FVector2D InRange(45.f,-45.f);
	FVector2D OutRange(-1.f,1.f);
	float DeltaYaw = DeltaRotator.Yaw;
	float Steering =  FMath::GetMappedRangeValueClamped(InRange,OutRange,DeltaYaw);
	return Steering;
}

void AAI_ChaosTank::AimAtEnemy()
{
	//Find LookAtRotation and Delta Rotator - Note that GetSocketLocation/Rotation can recieve the name of a Bone instead of a socket,
	//we will use this to ensure the LookAtRotation is based on the turret not the hull
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName()),EnemyLoc);
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(GetMesh()->GetSocketRotation(FName()),LookAtRotation);
	TurretRot = DeltaRotator;
	
}

void AAI_ChaosTank::Attack()
{
	//Attack Code here
}
