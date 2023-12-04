// Masters Project - Owen S Atkinson

#pragma once
#include "ArmourInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmourActor.generated.h"

UCLASS()
class MASTERSPROJECT_API AArmourActor : public AActor, public IArmourInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmourActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* ArmourPanel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* ArmourStand;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Thickness;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Angle;
	virtual float GetArmourValue() override;

};
