#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"

class AlchemyMath
{
public:
	static float GetGradeMultiplier(EItemGrade Grade)
	{
		switch (Grade)
		{
		case EItemGrade::EIG_S: return 1.5f;
		case EItemGrade::EIG_A: return 1.3f;
		case EItemGrade::EIG_B: return 1.1f;
		case EItemGrade::EIG_C: return 1.0f;
		case EItemGrade::EIG_D: return 0.8f;
		case EItemGrade::EIG_E: return 0.5f;
		default: return 1.0f;
		}
	}

	static int32 GetGradeScore(EItemGrade Grade)
	{
		switch (Grade)
		{
		case EItemGrade::EIG_S: return 100;
		case EItemGrade::EIG_A: return 80;
		case EItemGrade::EIG_B: return 60;
		case EItemGrade::EIG_C: return 40;
		case EItemGrade::EIG_D: return 20;
		case EItemGrade::EIG_E: return 10;
		default: return 0;
		}
	}

	static EItemGrade GetGradeFromScore(int32 Score)
	{
		if (Score >= 90) return EItemGrade::EIG_S;
		if (Score >= 70) return EItemGrade::EIG_A;
		if (Score >= 50) return EItemGrade::EIG_B;
		if (Score >= 30) return EItemGrade::EIG_C;
		if (Score >= 15) return EItemGrade::EIG_D;
		return EItemGrade::EIG_E;
	}
};