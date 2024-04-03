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

float MathHelper::DeMarresEquation(float RP, float V, float RV, float D, float RD, float W, float RW)
{
	/*Taken from WWII Ballistics - Armor and Gunnery, requires a reference to a similar shell to be accurate and assumes homogenous(steel) armour,
	 *will try to make one that does not later on. Useful for only AP/APCBC shells, rods and missiles need a different equation.
	 *Equation is:
	 * Armour Penetration = RefPen * (V/Ref V)^1.4283 * (D/Ref D)^1.0714 * (M/D^3)^0.7143 / (Ref W/Ref D)
	 * Ref prefix = Reference
	 * RP = Reference Penetration
	 * V = velocity
	 * D = Diameter of Shell
	 * M = Mass of Shell
	 * W = Weight of Shell
	 * 
	 */
	
	//TODO: Create an Overloaded function that uses the more complex version that I dont understand
	float Velocity = pow((V/RV),1.4283f);
	float Diameter = pow((D/RD),1.0714f);
	float WD = pow((W/pow(D,3.0f)),0.7143f);
	float RFD = pow((RW/pow(RD,3.0f)),0.7143f);
	return (RP * Velocity * Diameter * WD )/ RFD;
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
	//normal = normal from trace hit result
	//impactForwardVector = forward vector from projectile
	//note both inputs must already be normalised
	float DotP = FVector::DotProduct(normal,impactForwardVector);
	return FMath::RadiansToDegrees(acosf(-DotP));
	
}

float MathHelper::CalculateAngleofImpact(FVector normal, FVector impactForwardVector, float Calibre, float thickness)
{
	//overloaded equation for determining if the sloped armour will be effective.
	if(Calibre >(thickness * 7.0f))// if the calibre is bigger tham the armour thickness * 7.0f then it is too big to be effected by ricochet and sloped armour
	{
		return 0.0f;
	}
	else
	{
		float DotP = FVector::DotProduct(normal,impactForwardVector);
		return FMath::RadiansToDegrees(acosf(-DotP));
	}
}

float MathHelper::LineOfSightThickness(float L, float a)
{
	/*This function gives a quick and cheap way of calculating effective armour thickness, from what I can tell
	 * this is used in most tank based games, useful for gameplay and performance but can be inaccurate in some cases.
	* Llos = L / cos@
	* - Llos = Length line of sight (Line of sight thickness)
	* - L = armour plate thickness
	* - @ = angle of armour
	 */
	return L/cos(a);
}

float MathHelper::GetDistance(AActor* ActorA, AActor* ActorB)
{
	return (ActorA->GetActorLocation()-ActorB->GetActorLocation()).Size();
}

float MathHelper::CalculateBlastRadius(float BombMass)
{
	//Based on Hopkinson-Cranz Scaling law and assumes an Bare explosion. Can be checked against https://unsaferguard.org/un-saferguard/explosion-danger-area. Should be accurate within 1 or 2 metres
	//R=ZW^1/3
	//W = Mass, Z = Scaled Distance assumes TNT or TNT equivalent
	float MassCubed = std::cbrt(BombMass);
	return 130.f * MassCubed;
	
}


