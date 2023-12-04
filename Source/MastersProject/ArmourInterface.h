// Masters Project - Owen S Atkinson

#pragma once
#include"ArmourInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI,Blueprintable)
class UArmourInterface : public UInterface
{
 GENERATED_BODY()
};
class IArmourInterface
{
 GENERATED_BODY()
public:
 virtual float GetArmourValue();
};
