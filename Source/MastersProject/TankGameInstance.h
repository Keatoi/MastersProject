// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TankGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API UTankGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	UPROPERTY(EditAnywhere,Blueprintable,Category = "UX")
	TSubclassOf<UUserWidget> LoadingWidget;
};
