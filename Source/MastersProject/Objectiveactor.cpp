// Masters Project - Owen S Atkinson
#include "Objectiveactor.h"
#include "Components/SphereComponent.h"
#include "TankGameMode.h"
#include "TankGameState.h"


#include "Kismet/GameplayStatics.h"

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
	GM = Cast<ATankGameMode>(UGameplayStatics::GetGameMode(this));
	GS = Cast<ATankGameState>(UGameplayStatics::GetGameState(this));
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
	FTimerDelegate IncreaseDelegate = FTimerDelegate::CreateUObject(this,&AObjectiveactor::IncreaseTickets,CaptureTeamEnum);
	FTimerDelegate DecreaseDelegate = FTimerDelegate::CreateUObject(this,&AObjectiveactor::IncreaseTickets,CaptureTeamEnum);
	if(OtherActor->ActorHasTag("Blue") && CaptureTeamEnum != EBLU)
	{
		//Decrease Owning Time
		GetWorld()->GetTimerManager().SetTimer(DecapHandle,DecreaseDelegate,1.f,true);
	    CaptureTeamEnum = EBLU;
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle,IncreaseDelegate,2.f,true);
		
	}
	else if(OtherActor->ActorHasTag("Blue"))
	{
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle,IncreaseDelegate,2.f,true);
	}
	else if (OtherActor->ActorHasTag("Red") && CaptureTeamEnum != ERED)
	{
		GetWorld()->GetTimerManager().SetTimer(DecapHandle,DecreaseDelegate,1.f,true);
		CaptureTeamEnum = ERED;
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle,IncreaseDelegate,2.f,true);
	}
	else if (OtherActor->ActorHasTag("Red"))
	{
		CaptureTeamEnum = ERED;
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle,IncreaseDelegate,1.f,true);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(DecapHandle,DecreaseDelegate,1.f,true);
		CaptureTeamEnum = ENON;
		GetWorld()->GetTimerManager().SetTimer(CaptureHandle,IncreaseDelegate,1.f,true);
	}
}

void AObjectiveactor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AObjectiveactor::CompareCaptureScores()
{
	float RedScore = FMath::Clamp(RedCaptureScore,0.f,100.f);
	float BlueScore = FMath::Clamp(BlueCaptureScore,0.f,100.f);
	
}

void AObjectiveactor::IncreaseTickets(TEnumAsByte<ECaptureEnum> TickettoIncrease)
{
	if(GS)
	{
		switch(TickettoIncrease)
		{
			case(EBLU):
				GS->BlueTickets++;
			break;
			case(ERED):
				GS->RedTickets++;
			break;
		default:
			GS->NeutralTickets++;
			break;
		}
	}
}

void AObjectiveactor::DecreaseTickets(TEnumAsByte<ECaptureEnum> TickettoDecrease)
{
	if(GM)
	{
		switch(TickettoDecrease)
		{
		case(EBLU):
			GS->BlueTickets--;
			break;
		case(ERED):
			GS->RedTickets--;
			break;
		default:
			GS->NeutralTickets--;
			break;
		}
	}
}

