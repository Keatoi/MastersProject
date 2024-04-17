// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objectiveactor.generated.h"
UENUM(BlueprintType)
enum ECaptureEnum
{
	EBLU UMETA(DisplayName = "Blue Team"),
	ERED UMETA(DisplayName = "Red Team"),
	ENON UMETA(DisplayName = "Neutral")
};
UCLASS()
class MASTERSPROJECT_API AObjectiveactor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveactor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
UPROPERTY(	EditAnywhere,BlueprintReadWrite,Category="Obj")
	USceneComponent* ObjectiveRoot;
	UPROPERTY(	EditAnywhere,BlueprintReadWrite,Category="Obj")
	class USphereComponent* CaptureZone;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Obj")
	class ATankGameMode* GM;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Obj")
	class ATankStateBase* GS;
    UPROPERTY()
	FTimerHandle RedCaptureHandle;
	UPROPERTY()
	FTimerHandle BlueCaptureHandle;
	UPROPERTY()
	FTimerHandle RedDecapHandle;
	UPROPERTY()
	FTimerHandle BlueDecapHandle;
	UPROPERTY(	EditAnywhere,BlueprintReadWrite,Category="Scoring")
	int BlueCaptureScore = 0.f;
	UPROPERTY(	EditAnywhere,BlueprintReadWrite,Category="Scoring")
	int RedCaptureScore = 0.f;
	UPROPERTY(	EditAnywhere,BlueprintReadWrite,Category="Scoring")
	TEnumAsByte<ECaptureEnum> CaptureTeamEnum = ECaptureEnum::ENON;// By Default zone is owned by none, but allow default team to be changed for greater flexibility in different game modes
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void CompareCaptureScores();
	UFUNCTION()
	void IncreaseBlueScore();
	UFUNCTION()
	void DecreaseBlueScore();
	UFUNCTION()
	void IncreaseRedScore();
	UFUNCTION()
	void DecreaseRedScore();
};
