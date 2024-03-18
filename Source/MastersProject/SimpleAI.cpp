// Masters Project - Owen S Atkinson


#include "SimpleAI.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASimpleAI::ASimpleAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = GetMesh();
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Tank Vision"));
	PawnSense->SetPeripheralVisionAngle(125.f);

	PawnSense->SensingInterval = .5f;//Sense every 0.25 seconds
}

// Called when the game starts or when spawned
void ASimpleAI::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSense->OnSeePawn.AddDynamic(this, &ASimpleAI::OnSeePawn);
	PawnSense->OnHearNoise.AddDynamic(this, &ASimpleAI::OnHearNoise);
	
}

UBehaviorTree* ASimpleAI::GetBehaviourTree() const
{
	return Tree;
}

void ASimpleAI::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	const FString VolumeDesc = FString::Printf(TEXT(" at volume %f"), Volume);
	FString message = TEXT("Heard Actor ") + OtherActor->GetName() + VolumeDesc;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

void ASimpleAI::OnSeePawn(APawn* OtherPawn)
{
	FString message = TEXT("Saw Actor ") + OtherPawn->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);
}

// Called every frame
void ASimpleAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



