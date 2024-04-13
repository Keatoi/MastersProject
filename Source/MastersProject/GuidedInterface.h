// Masters Project - Owen S Atkinson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GuidedInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGuidedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTERSPROJECT_API IGuidedInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTargetLocation(FVector TargetLocation);
};
