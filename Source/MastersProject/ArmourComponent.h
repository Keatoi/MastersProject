// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmourComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTERSPROJECT_API UArmourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArmourComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
 UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* ArmourPanel;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ArmourThickness;
		
};
