// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "Drone/DroneBase.h"
#include "PlayerDrone.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDistanceChangesSignature, bool, float);

class UCameraComponent;

UCLASS(Blueprintable)
class XYZHOMEWORK_API APlayerDrone : public ADroneBase
{
	GENERATED_BODY()

public:
	APlayerDrone();

	virtual void Tick(float DeltaSeconds) override;

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	virtual void MoveUp(float Value) override;
	virtual void Turn(float Value) override;
	virtual void LookUp(float Value) override;
	virtual void TurnAtRate(float Value) override;
	virtual void LookUpAtRate(float Value) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = -360.0, UIMin = -360.0, ClampMax = 360.0, UIMax = 360.0))
	float MaxCameraPitch = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (ClampMin = -360.0, UIMin = -360.0, ClampMax = 360.0, UIMax = 360.0))
	float MinCameraPitch = -90.0f;

private:
	void RotateCameraPitch(float Value);

	

	// =========================================
	// Info
	// =========================================

public:
	FOnDistanceChangesSignature OnDistanceChanges;

	virtual void UpdateDroneInfo() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Line Trace", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxDistanceLineDistance = 5000.0f;

private:
	float CurrentDistance = -1.0f;

	/** 
	 *	Calculate distance.
	 *	@param bForceBroadcast Should force broadcast OnDistanceChanges.
	 */
	void CalculateDistance(bool bForceBroadcast = false);

	// =========================================
	// ~Info
	// =========================================

	

	// =========================================
	// Components
	// =========================================

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	// =========================================
	// ~Components
	// =========================================
};
