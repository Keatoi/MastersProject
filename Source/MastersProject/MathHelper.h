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
	float DeMarresEquation(float RP, float V, float RV, float D, float RD, float W, float RW);
	UFUNCTION(BlueprintCallable)
	float CalculateRelativeArmourThickness(float ActualThickness,float AttackAngle);
	UFUNCTION(BlueprintCallable)
	float CalculateAngleofImpact(FVector normal, FVector impactForwardVector);
	UFUNCTION(BlueprintCallable)
	float CalculateAngleofImpact(FVector normal, FVector impactForwardVector, float Calibre,float thickness);
	UFUNCTION(BlueprintCallable)
	float LineOfSightThickness(float L, float a);
};
