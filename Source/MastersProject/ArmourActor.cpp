// Masters Project - Owen S Atkinson


#include "ArmourActor.h"

// Sets default values
AArmourActor::AArmourActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ArmourPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armour Panel"));
	ArmourStand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armour Stand"));
	SetRootComponent(ArmourStand);
	ArmourPanel->SetupAttachment(ArmourStand);
	
}

// Called when the game starts or when spawned
void AArmourActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmourActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AArmourActor::GetArmourValue()
{
	return Thickness;
}

