// Masters Project - Owen S Atkinson


#include "AI_ChaosTank.h"

#include "Camera/CameraComponent.h"
#include "Perception/PawnSensingComponent.h"

AAI_ChaosTank::AAI_ChaosTank()
{
	TestPlayerCamera->SetActive(false);
	GunnerCamera->SetActive(false);
	ZoomCamera->SetActive(false);
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Tank Vision"));
	PawnSense->SetPeripheralVisionAngle(125.f);
	
	PawnSense->SensingInterval = .25f;//Sense every 0.25 seconds
	
}

void AAI_ChaosTank::BeginPlay()
{
	Super::BeginPlay();
	PawnSense->OnSeePawn.AddDynamic(this,&AAI_ChaosTank::OnSeePawn);
	PawnSense->OnHearNoise.AddDynamic(this,&AAI_ChaosTank::OnHearNoise);
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
}
