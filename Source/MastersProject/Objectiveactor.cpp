// Masters Project - Owen S Atkinson
#include "Objectiveactor.h"
#include "Components/SphereComponent.h"
#include "TankGameMode.h"
#include "TankGameState.h"
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
	GM = Cast<ATankGameMode>(UGameplayStatics::GetGameMode(this));
	GS = Cast<ATankStateBase>(UGameplayStatics::GetGameState(this));
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
	}
	else if (RedCaptureScore > BlueCaptureScore)
	{
		//Red team has more points than Blue so set to red
		CaptureTeamEnum = ERED;
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
}



