#pragma once

namespace XHMathUtils
{
	
	FVector GetMiddleBetweenTwoPoints(const FVector& V1, const FVector& V2);
	FRotator GetTargetRotation(const FVector& ObjectLocation, const FVector& TargetLocation, EAxis::Type LookingAxis, bool bLockPitch = false, bool bLockYaw = false, bool bLockRoll = false);

	void LoopIncrement(int32& number, int32 min, int32 max);
	void LoopIncrement(int32& number, int32 max);

	void LoopDecrement(int32& number, int32 min, int32 max);
};
