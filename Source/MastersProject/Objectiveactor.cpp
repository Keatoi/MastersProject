// Masters Project - Owen S Atkinson
#include "Objectiveactor.h"
#include "Components/SphereComponent.h"
#include "TankGameMode.h"

#include "TankStateBase.h"


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
	CaptureZone->OnComponentBeginOverlap.AddDynamic(this,&AObjectiveactor::OnOverlapBegin);
	CaptureZone->OnComponentEndOverlap.AddDynamic(this,&AObjectiveactor::OnOverlapEnd);
	UWorld* World = GetWorld();
	if(World)
	{
		GM = Cast<ATankGameMode>(UGameplayStatics::GetGameMode(World));
		GS = Cast<ATankStateBase>(UGameplayStatics::GetGameState(World));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO World"));
	}
	
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
	CompareCaptureScores();
}

void AObjectiveactor::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Entering Objective: %s"),*OtherActor->GetName());
	if(OtherActor->ActorHasTag("Blue"))
	{
		GetWorld()->GetTimerManager().SetTimer(BlueCaptureHandle,this,&AObjectiveactor::IncreaseBlueScore,3.f,true,1.f);
		GetWorld()->GetTimerManager().SetTimer(RedDecapHandle,this,&AObjectiveactor::DecreaseRedScore,2.5f,true,2.f);
	}
	else if(OtherActor->ActorHasTag("Red"))
	{
		GetWorld()->GetTimerManager().SetTimer(RedCaptureHandle,this,&AObjectiveactor::IncreaseRedScore,3.f,true,1.f);
		GetWorld()->GetTimerManager().SetTimer(BlueDecapHandle,this,&AObjectiveactor::DecreaseBlueScore,2.5f,true,2.f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : Has no team tag"), *OtherActor->GetName());
	}
}

void AObjectiveactor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Blue"))
	{
		GetWorld()->GetTimerManager().ClearTimer(BlueCaptureHandle);
		GetWorld()->GetTimerManager().ClearTimer(RedDecapHandle);
	}
	else if(OtherActor->ActorHasTag("Red"))
	{
		GetWorld()->GetTimerManager().ClearTimer(RedCaptureHandle);
		GetWorld()->GetTimerManager().ClearTimer(BlueDecapHandle);
	}
}

void AObjectiveactor::CompareCaptureScores()
{
	//clamp both scores to 100 so that one team cant get an absurdly high score just by camping in the objective area
	float RedScore = FMath::Clamp(RedCaptureScore,0.f,100.f);
	float BlueScore = FMath::Clamp(BlueCaptureScore,0.f,100.f);
	if(BlueCaptureScore > RedCaptureScore)
	{
		//Blue team has more points so set to blue
		CaptureTeamEnum = EBLU;
		
		//GS->DecreaseRed(50.f);
	}
	else if (RedCaptureScore > BlueCaptureScore)
	{
		//Red team has more points than Blue so set to red
		CaptureTeamEnum = ERED;
		
		
		//GS->DecreaseBlue(50.f);
	}
	else
	{
		// Teams are at a draw or haven't made a capture attempt, set to neutral
		CaptureTeamEnum = ENON;
	}
	
}

void AObjectiveactor::IncreaseBlueScore()
{
	BlueCaptureScore += 10.f;
	UE_LOG(LogTemp,Warning,TEXT("Blue Score: %d"),BlueCaptureScore);
	
}

void AObjectiveactor::DecreaseBlueScore()
{
	BlueCaptureScore -= 5.f;
	UE_LOG(LogTemp,Warning,TEXT("Blue Score: %d"),BlueCaptureScore);
	if(GS)
	{
		GS->DecreaseBlue(5.f);
		UE_LOG(LogTemp,Warning,TEXT("Blue Tickets: %f"),GS->BlueTickets);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GS"));
	}
}

void AObjectiveactor::IncreaseRedScore()
{
	RedCaptureScore += 10.f;
	UE_LOG(LogTemp,Warning,TEXT("Red Score: %d"),RedCaptureScore);
	
}

void AObjectiveactor::DecreaseRedScore()
{
	RedCaptureScore -= 5.f;
	UE_LOG(LogTemp,Warning,TEXT("Red Score: %d"),RedCaptureScore);
	if(GS)
	{
		GS->DecreaseRed(5.f);
		UE_LOG(LogTemp,Warning,TEXT("Red Tickets: %f"),GS->RedTickets);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GS"));
	}
}



