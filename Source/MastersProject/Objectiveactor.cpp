// Masters Project - Owen S Atkinson

#include "Components/SphereComponent.h"
#include "Objectiveactor.h"

// Sets default values
AObjectiveactor::AObjectiveactor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectiveRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(ObjectiveRoot);
	CaptureZone = CreateDefaultSubobject<USphereComponent>(TEXT("CaptureZone"));
	CaptureZone->SetSphereRadius(5000.f);
	CaptureZone->SetupAttachment(ObjectiveRoot);
}

// Called when the game starts or when spawned
void AObjectiveactor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectiveactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectiveactor::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FTimerDelegate IncreaseDelegate = FTimerDelegate::CreateUObject(this,&AObjectiveactor::IncreaseTickets,OwningTeamEnum);
	
	if(OtherActor->ActorHasTag("Blue"))
	{
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle,IncreaseDelegate,1.f,true);
		IncreaseTickets(ECaptureEnum::EBLU);
		DecreaseTickets(ECaptureEnum::ERED);
	}
	else if (OtherActor->ActorHasTag("Red"))
	{
		IncreaseTickets(ECaptureEnum::EBLU);
		DecreaseTickets(ECaptureEnum::ERED);
	}
}

void AObjectiveactor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AObjectiveactor::CompareCaptureScores()
{
	
}

void AObjectiveactor::IncreaseTickets(TEnumAsByte<ECaptureEnum> TickettoIncrease)
{
	
}

void AObjectiveactor::DecreaseTickets(TEnumAsByte<ECaptureEnum> TickettoDecrease)
{
}

