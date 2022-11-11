// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "XHTypes.h"
#include "GameFramework/Pawn.h"
#include "DroneBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDroneDestroyedSignature, class ADroneBase*);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputAccelerationChangesSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSpeedChangesSignature, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRollAngleChangesSignature, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAltitudeChangesSignature, bool, float);

class UCapsuleComponent;
class UFloatingPawnMovement;
class UHealthComponent;
class UExplosionComponent;
class USkeletalMeshComponent;
class UArrowComponent;
class UDroneControllerComponent;

UCLASS(Abstract)
class XYZHOMEWORK_API ADroneBase : public APawn, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADroneBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void UnPossessed() override;

	/** 
	 *	Do self destruction.
	 *	@see UExplosionComponent
	 */
	void SelfDestruct();

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void MoveUp(float Value);
	virtual void Turn(float Value);
	virtual void LookUp(float Value);
	virtual void TurnAtRate(float Value);
	virtual void LookUpAtRate(float Value);

	void ShowDroneAndEnableCollision();
	void HideDroneAndDisableCollision();

	FORCEINLINE void SetIsActive(bool bIsActive_In) { bIsActive = bIsActive_In; }
	
	/** Is drone working. */
	FORCEINLINE bool IsActive() const { return bIsActive; }

	/** Broadcasting when drone has been destroyed (by damage or by itself). */
	FOnDroneDestroyedSignature OnDroneDestroyed;

protected:
	/** Rate for turning. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float BaseTurnRate = 90.0f;

	/** Rate for looking up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float BaseLookUpRate = 2.0f;

	/** VFX for drone death. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death", meta = (EditCondition="!bUseExplosionComponentOnDeath", EditConditionHides))
	UParticleSystem* DeathVFX;

	/** Max pitch angle tilt when drone moves. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone Tilt", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 90.0, UIMax = 90.0))
	float MaxTiltPitchAngle = 5.0f;

	/** Max roll angle tilt when drone moves. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone Tilt", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 90.0, UIMax = 90.0))
	float MaxTiltRollAngle = 5.0f;

	/** Interpolation tilt speed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drone Tilt", meta = (ClampMin = 0.0, UIMin = 0.0))
	float TiltSpeed = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	ETeams Team = ETeams::Player;
	
	virtual void BeginPlay() override;

private:
	// Cached forward axis input.
	float ForwardAxis = 0.0f;
	
	// Cached right axis input.
	float RightAxis = 0.0f;
	
	// Cached up axis input.
	float UpAxis = 0.0f;

	// Is drone active.
	bool bIsActive = false;

	/**
	 *	Calls when drone has died.
	 *	@see UHealthComponent
	 */
	void OnDroneDeath();

	/**
	 * Tilts the drone on moving.
	 * @param DeltaSeconds Game time elapsed during last frame modified by the time dilation.
	 */
	void TiltDrone(float DeltaSeconds);


	
	// =========================================
	// Info
	// =========================================

public:
	FOnInputAccelerationChangesSignature OnInputAccelerationChanges;
	FOnSpeedChangesSignature OnSpeedChanges;
	FOnRollAngleChangesSignature OnRollAngleChanges;
	FOnAltitudeChangesSignature OnAltitudeChanges;

	/** Recalculate info and force broadcasting. */
	virtual void UpdateDroneInfo();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Line Trace")
	bool bDisableLineTraceOnUnPossess = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Line Trace", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float MaxAltitudeLineDistance = 5000.0f;
	
private:
	float CurrentAcceleration = 0.0f;
	float CurrentVelocity = 0.0f;
	float CurrentAltitude = -1.0f;

	/** 
	 *	Calculate altitude.
	 *	@param bForceBroadcast Should force broadcast OnAltitudeChanges.
	 */
	void CalculateAltitude(bool bForceBroadcast = false);

	/** 
	 *	Update input acceleration.
	 *	@param bForceBroadcast Should force broadcast OnInputAccelerationChanges.
	 */
	void UpdateInputAcceleration(bool bForceBroadcast = false);

	/** 
	 *	Update velocity.
	 *	@param bForceBroadcast Should force broadcast OnSpeedChanges.
	 */
	void UpdateVelocity(bool bForceBroadcast = false);

	// =========================================
	// ~Info
	// =========================================

	
	
	// =========================================
	// Components
	// =========================================
	
public:
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
	FORCEINLINE USkeletalMeshComponent* GetMeshComponent() const { return MeshComponent; }
	FORCEINLINE UFloatingPawnMovement* GetFloatingPawnMovement() const	{ return FloatingPawnMovement; }
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	FORCEINLINE UExplosionComponent* GetExplosionComponent() const { return ExplosionComponent; }

private:
	/** The CapsuleComponent being used for movement collision. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
	
	/** The main static mesh associated with this Drone. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	/** Movement component used for movement logic. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovement;

	/** Health component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	/** Explosion component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess = "true"))
	UExplosionComponent* ExplosionComponent;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate drone facing. */
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif
	
	// =========================================
	// ~Components
	// =========================================


	
	// =========================================
	// IGenericTeamAgentInterface
	// =========================================

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

	// =========================================
	// ~IGenericTeamAgentInterface
	// =========================================

	
	
	// =========================================
	// Network
	// =========================================

private:
	/**
	 * TODO Move movement replication to movement component
	 */
	UFUNCTION(Server, Unreliable)
	void Server_UpdatePawnTransform(const FTransform& NewTransform);

	UFUNCTION(Server, Unreliable)
	void Server_UpdateMeshTilt(const FRotator& NewRotation);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateMeshTilt(const FRotator& NewRotation);

	UFUNCTION(Server, Reliable)
	void Server_SelfDestruct();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SelfDestruct();

	UFUNCTION(Server, Reliable)
	void Server_OnDroneDeath();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDroneDeath();

	// =========================================
	// ~Network
	// =========================================
};


