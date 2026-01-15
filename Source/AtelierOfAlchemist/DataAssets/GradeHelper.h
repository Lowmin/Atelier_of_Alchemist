// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"

/**
 * 
 */
class AlchemyMath
{
public:
	static int32 GetGradeScore(EItemGrade Grade)
	{
		switch (Grade)
		{
		case EItemGrade::EIG_S: return 5;
		case EItemGrade::EIG_A: return 4;
		case EItemGrade::EIG_B: return 3;
		case EItemGrade::EIG_C: return 2;
		case EItemGrade::EIG_D: return 1;
		default: return 0;
		}
	}
	static EItemGrade GetGradeFromScore(int32 Score)
	{
		if (Score >= 5) return EItemGrade::EIG_S;
		else if (Score >= 4) return EItemGrade::EIG_A;
		else if (Score >= 3) return EItemGrade::EIG_B;
		else if (Score >= 2) return EItemGrade::EIG_C;
		else if (Score >= 1) return EItemGrade::EIG_D;
		else return EItemGrade::EIG_E;
	}
	static float GetStatMultiplier(EItemGrade Grade)
	{
		switch (Grade)
		{
		case EItemGrade::EIG_S: return 1.5f;
		case EItemGrade::EIG_A: return 1.2f;
		case EItemGrade::EIG_B: return 1.0f;
		case EItemGrade::EIG_C: return 0.8f;
		case EItemGrade::EIG_D: return 0.6f;
		default: return 0.5f;
		}
	}
};
