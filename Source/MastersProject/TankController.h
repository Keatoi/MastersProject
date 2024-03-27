// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GenericTeamAgentInterface.h"
#include "TankController.generated.h"

class AChaosTankPawn;
/**
 * 
 */
UCLASS()
class MASTERSPROJECT_API ATankController : public APlayerController,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ATankController();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	FGenericTeamId TeamID = FGenericTeamId(1);
	virtual FGenericTeamId GetGenericTeamId() const override;
	UPROPERTY()
	TObjectPtr<class AChaosTankPawn> PC;
	//InputMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;
	//InputActions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputTurn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputFirePrimary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputFireSecondary;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputReloadIM;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputLook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputCameraSwap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputDefaultCam;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputZoomCam;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputGunnerCam;
	//Input Functions
	UFUNCTION()
	void MoveTriggered(const FInputActionValue& Value);
	UFUNCTION()
	void MoveCompleted(const FInputActionValue& Value);
	UFUNCTION()
	void MoveStarted(const FInputActionValue& Value);
	UFUNCTION()
	void MoveCancelled(const FInputActionValue& Value);
	UFUNCTION()
	void TurnTriggered(const FInputActionValue& Value);
	UFUNCTION()
	void TurnStarted(const FInputActionValue& Value);
	UFUNCTION()
	void TurnCancelled(const FInputActionValue& Value);
	UFUNCTION()
	void TurnCompleted(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void PrimaryFire(const FInputActionValue& Value);
	UFUNCTION()
	void SecondaryFireStart(const FInputActionValue& Value);
	UFUNCTION()
	void SecondaryFire();
	UFUNCTION()
	void SecondaryFireReleased(const FInputActionValue& Value);
	UFUNCTION()
	void CameraSwap(const FInputActionValue& Value);
	UFUNCTION()
	void DefaultView(const FInputActionValue& Value);
	UFUNCTION()
	void CommanderView(const FInputActionValue& Value);
	UFUNCTION()
	void GunnerView(const FInputActionValue& Value);
	UFUNCTION()
	void ReloadMG();
	UFUNCTION()
	void ReloadInteriorMagazine(const FInputActionValue& Value);//Reload the InteriorMagazine

};
