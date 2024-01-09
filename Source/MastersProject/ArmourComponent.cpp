// Masters Project - Owen S Atkinson


#include "ArmourComponent.h"

// Sets default values for this component's properties
UArmourComponent::UArmourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	ArmourPanel = CreateDefaultSubobject<UStaticMeshComponent>("Armour Panel Mesh");
}


// Called when the game starts
void UArmourComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UArmourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

