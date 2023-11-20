// Fill out your copyright notice in the Description page of Project Settings.


#include "MathHelper.h"



MathHelper::MathHelper()
{
}

MathHelper::~MathHelper()
{
}

float MathHelper::KruppsEquation(float V, float P, float D, float K)
{
	//Krupp's Equation- Useful for ballpark estimates. Assumes 90 degree impact. 
	/*
	 * B = Armour Penetrated
	 * V = Velocity
	 * P = Projectile Mass
	 * D = Calibre
	 * K = Resistance Constant (if unsure assume 2400 -  the standard RC used by the USSR in Pen Testing)
	 */
	float B = (V * sqrt(P))/(K * sqrt(D));
	//divide result by 100 to get result in mm instead of dm
	return B/100;
}

float MathHelper::KruppsEquation(float V, float P, float D)
{
	//overloaded function if K is unknown. See above for legend.
	float B = (V * sqrt(P))/(2400 * sqrt(D));
	//divide result by 100 to get result in mm instead of dm
	return B/100;
}

float MathHelper::DeMarresEquation(float Mass, float Thickness, float Diameter, float V, float Deg, float K, float n,
	float s)
{
	float D = Diameter * 100; //Diameter is normally in mm but equation requires dm so multiply by 100
	float KSqr = K * K; //slightly faster than using pow
	float DCube = D * D * D;
	//TODO: Finish the equation after I figure out how to do said equation
	return 0;
}

float MathHelper::CalculateRelativeArmourThickness(float ActualThickness, float AttackAngle)
{
	/*C = a / cos(b)
 * a = actual armour thickness
 * b = impact angle
 * c = relative armour thickness
 */
	return ActualThickness / cos(AttackAngle);
}

float MathHelper::CalculateAngleofImpact(FVector normal, FVector impactForwardVector)
{
	//note both inputs must already be normalised
	float DotP = FVector::DotProduct(normal,impactForwardVector);
	return FMath::RadiansToDegrees(acosf(-DotP));
	
}


