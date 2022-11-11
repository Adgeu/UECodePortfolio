#include "XHMathUtils.h"

FVector XHMathUtils::GetMiddleBetweenTwoPoints(const FVector& V1, const FVector& V2)
{
	return  FVector((V1.X + V2.X) / 2, (V1.Y + V2.Y) / 2, (V1.Z + V2.Z) / 2);
}

FRotator XHMathUtils::GetTargetRotation(const FVector& ObjectLocation, const FVector& TargetLocation, EAxis::Type LookingAxis, bool bLockPitch, bool bLockYaw, bool bLockRoll)
{
	FRotator Result = FRotator::ZeroRotator;
	
	if (LookingAxis == EAxis::X)
	{
		Result = FRotationMatrix::MakeFromX(TargetLocation - ObjectLocation).Rotator();
	}
	else if (LookingAxis == EAxis::Y)
	{
		Result = FRotationMatrix::MakeFromY(TargetLocation - ObjectLocation).Rotator();
	}
	else if (LookingAxis == EAxis::Z)
	{
		Result = FRotationMatrix::MakeFromZ(TargetLocation - ObjectLocation).Rotator();
	}
	
	return FRotator(bLockPitch ? 0.0f : Result.Pitch, bLockYaw ? 0.0f : Result.Yaw, bLockRoll ? 0.0f : Result.Roll);
}

void XHMathUtils::LoopIncrement(int32& number, int32 min, int32 max)
{
	number++;
	if (number >= max)
	{
		number = min;
	}
}

void XHMathUtils::LoopIncrement(int32& number, int32 max)
{
	LoopIncrement(number, 0, max);
}

void XHMathUtils::LoopDecrement(int32& number, int32 min, int32 max)
{
	number--;
	if (number <= min)
	{
		number = max;
	}
}
