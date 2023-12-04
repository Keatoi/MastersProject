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
	/*================Penetration Functions============================================*/
	UFUNCTION(BlueprintCallable)
	static float KruppsEquation(float V, float P, float D, float K);
	UFUNCTION(BlueprintCallable)
	static float KruppsEquation(float V, float P, float D);
	UFUNCTION(BlueprintCallable)
	static float DeMarresEquation(float RP, float V, float RV, float D, float RD, float W, float RW);
	UFUNCTION(BlueprintCallable)
	static float CalculateRelativeArmourThickness(float ActualThickness,float AttackAngle);
	UFUNCTION(BlueprintCallable)
	static float CalculateAngleofImpact(FVector normal, FVector impactForwardVector);
	UFUNCTION(BlueprintCallable)
	static auto CalculateAngleofImpact(FVector normal, FVector impactForwardVector, float Calibre,
	                                   float thickness) -> float;
	UFUNCTION(BlueprintCallable)
	static float LineOfSightThickness(float L, float a);
	/*================Generic Math Functions============================================*/
	/**
	 * 
	 */
	
};
