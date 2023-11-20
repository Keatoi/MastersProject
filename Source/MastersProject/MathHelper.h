// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MASTERSPROJECT_API MathHelper
{
public:
	MathHelper();
	~MathHelper();
	UFUNCTION(BlueprintCallable)
	float KruppsEquation(float V, float P, float D, float K);
	UFUNCTION(BlueprintCallable)
	float KruppsEquation(float V, float P, float D);
	UFUNCTION(BlueprintCallable)
	float DeMarresEquation(float Mass, float Thickness, float Diameter, float V,float Deg,float K, float n, float s);
	UFUNCTION(BlueprintCallable)
	float CalculateRelativeArmourThickness(float ActualThickness,float AttackAngle);
	UFUNCTION(BlueprintCallable)
	float CalculateAngleofImpact(FVector normal, FVector impactForwardVector);
};
