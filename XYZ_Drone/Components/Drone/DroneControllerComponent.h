// For educational purposes only. 

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DroneControllerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAvailableDroneCountChangesSignature, int32);

class ADroneBase;
class UAnimMontage;
class UDroneWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XYZHOMEWORK_API UDroneControllerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UDroneControllerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ADroneBase* GetCurrentConnectedDrone() const;

	/**
	 * Start launch inactive drone.
	 * @param bConnectToDroneAfterLaunching Should connect to drone after launching.
	 */
	void StartLaunchDrone(bool bConnectToDroneAfterLaunching = false);
	
	/**
	 * Launch inactive drone.
	 */
	void LaunchDrone();

	/**
	 * Connect to the last active drone.
	 * @see bShouldLaunchOnFailedConnection
	 */
	void ConnectToLastDrone();

	/**
	 * Switch to the next active drone.
	 * @return Successful connection.
	 */
	bool TryConnectToNextDrone();

	/**
	 * Switch to the previous active drone.
	 * @return Successful connection.
	 */
	bool TryConnectToPreviousDrone();

	/**
	 * Disconnect active drone and return control to owning character.
	 */
	void DisconnectDrone();

	/**
	 * Disconnect and deactivate active drone.
	 */
	void DeactivateDrone();

	/**
	 *	Checks the possibility of launching a drone.
	 *	@return We can launch a drone.
	 */
	bool CanLaunch() const;

	/**
	 * Is current active drone valid.
	 */
	bool IsControllingDroneNow() const;

	/** Available drone count has changed. */
	FOnAvailableDroneCountChangesSignature OnAvailableDroneCountChanges;

protected:
	virtual void BeginPlay() override;

	/** The drone class that we need to spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Settings")
	TSubclassOf<ADroneBase> DroneClass;

	/** Max available drone count. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Settings", meta = (ClampMin = 0, UIMin = 0))
	int32 MaxDroneCount = 3;

	/** Max count in drone pool. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Settings", meta = (ClampMin = 1, UIMin = 1))
	int32 DronePoolSize = 3;

	/** 
	 *	[TRUE] - If there is no available drone to connect, we trying to launch a new drone and then connecting to this one. \n
	 *	[FALSE] - If there is no available drone to connect, we stay in owner pawn.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Settings")
	bool bShouldLaunchOnFailedConnection = true;

	/**
	 *	[TRUE] - If the drone was destroyed, try to connect to an available drone. \n
	 *	[FALSE] - If the drone was destroyed, possess to the owner pawn.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drone Settings")
	bool bShouldConnectToAvailableDroneOnDestroy = true;

	/** This value added to capsule height, when there is a spawn check. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Settings", meta = (ClampMin = 0.0, UIMin = 0.0))
	float HeightSpawnOffset = 5.0f;

	/** This value added to capsule radius, when there is a spawn check. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Settings", meta = (ClampMin = 0.0, UIMin = 0.0))
	float RadiusSpawnOffset = 2.0f;

	/**
	 *	Launching animation.
	 *	@note Should contains LaunchDrone animation notify.
	 *	@see AnimNotify_LaunchDrone
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* LaunchAnimMontage;

private:
	// Casted owner.
	TWeakObjectPtr<ACharacter> CachedOwnerCharacter;

	// Current active connected drone.
	UPROPERTY(Replicated)
	ADroneBase* CurrentConnectedDrone = nullptr;

	// Drone pool.
	UPROPERTY(Replicated)
	TArray<ADroneBase*> DronePool;

	// Active drones that we can connect to.
	UPROPERTY(Replicated)
	TArray<ADroneBase*> ActiveDrones;

	// Current drone pool index.
	UPROPERTY(Replicated)
	int32 CurrentDronePoolIndex = 0;

	// Spawn drone pool location.
	const FVector DronePoolLocation = FVector(0.0f, 0.0f, -100.0f);

	// Available drone count.
	UPROPERTY(ReplicatedUsing=OnRep_AvailableDroneCount)
	int32 AvailableDroneCount = 0;

	// Timer handle for connection to the launching drone (if we have got LaunchingAnimation).
	FTimerHandle ConnectDroneTimer;

	/**
	 * [Internal] Start launch inactive drone.
	 * @param bConnectToDroneAfterLaunching Should connect to drone after launching.
	 */
	void StartLaunchDrone_Internal(bool bConnectToDroneAfterLaunching);

	/**
	 * Set new value to available drone count.
	 * @param Value New AvailableDroneCount value.
	 */
	FORCEINLINE void SetAvailableDroneCount(int32 Value) { AvailableDroneCount = FMath::Clamp(Value, 0, MaxDroneCount); }

	/** Initialize drone pool. */
	void FillDronePool();

	/**
	 * Get drone from drone pool that is not in ActiveDrones.
	 * @return Valid inactive Drone.
	 */
	ADroneBase* TryGetUnusedDrone();

	/** Do spawn check. */
	bool CanSpawnDrone() const;

	/** Drone was destroyed. */
	void OnDroneDestroyed(ADroneBase* DestroyedDrone);
	
	void PossessDrone(AController* Controller, ADroneBase* Drone);
	void PossessOwnerPawn(AController* Controller);


	
	// =========================================
	// Drone Widget
	// =========================================

public:
	void SetDroneWidgetVisibility(bool bWidgetVisible);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UDroneWidget> DroneWidgetClass;
	
private:
	UDroneWidget* DroneWidget = nullptr;
	
	// =========================================
	// ~Drone Widget
	// =========================================

	

	// =========================================
	// Network
	// =========================================

private:
	UFUNCTION()
	void OnRep_AvailableDroneCount();

	UFUNCTION(Server, Reliable)
	void Server_StartLaunchDrone(bool bConnectToDroneAfterLaunching);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartLaunchDrone(bool bConnectToDroneAfterLaunching);
	
	UFUNCTION(Server, Reliable)
	void Server_LaunchDrone();

	UFUNCTION(Server, Reliable)
	void Server_ConnectToLastDrone();

	UFUNCTION(Server, Reliable)
	void Server_DisconnectDrone();

	UFUNCTION(Server, Reliable)
	void Server_ConnectToNextDrone();

	UFUNCTION(Server, Reliable)
	void Server_ConnectToPreviousDrone();

	// =========================================
	// ~Network
	// =========================================
};
