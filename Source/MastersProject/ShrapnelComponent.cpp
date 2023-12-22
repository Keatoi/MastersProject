// Masters Project - Owen S Atkinson


#include "ShrapnelComponent.h"

// Sets default values for this component's properties
UShrapnelComponent::UShrapnelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShrapnelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShrapnelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShrapnelComponent::ShrapnelFire(int ShrapnelNum, float HalfAngle)
{
	//Make the cone
	const int32 RandomSeed = FMath::Rand();
	FRandomStream ShrapnelStream(RandomSeed);
	
}

