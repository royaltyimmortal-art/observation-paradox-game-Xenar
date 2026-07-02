#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Core/QuantumObservationSystem.h"
#include "PlayerPerceptionSystem.generated.h"

/**
 * PLAYER PERCEPTION SYSTEM
 * 
 * Tracks what the player is looking at and how intently:
 * - Camera direction and focus
 * - Peripheral vision vs direct observation
 * - Observation duration and intensity
 * - "Noticing" thresholds for detecting anomalies
 * - Attention capacity (can't observe everything at once)
 * 
 * This feeds directly into the Quantum Observation System
 */

ENUM(uint8, EPeripheralVision)
{
    EPV_DirectFocus,     // Looking directly at something
    EPV_Peripheral,      // In edge of vision
    EPV_Barely,          // Barely noticeable
    EPV_NotVisible       // Not in field of view
};

STRUCT_TYPE(FPerceptionRay)
{
    GENERATED_BODY()

    FVector RayStart;
    FVector RayDirection;
    float RayLength;
    float FocusIntensity;  // 0.0 to 1.0
    EPeripheralVision PeripheralType;
};

STRUCT_TYPE(FObservedObject)
{
    GENERATED_BODY()

    AActor* ObjectActor;
    float ObservationDuration;
    float FocusIntensity;
    FVector LastSeenLocation;
    bool bHasBeenSeen;
    float TimeSinceLastSeen;
};

UCLASS()
class OBSERVATIONPARADOX_API UPlayerPerceptionSystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    UPlayerPerceptionSystem();
    
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    
    // Camera/viewing setup
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void SetViewport(const FVector& CameraLocation, const FRotator& CameraRotation);
    
    // Main perception update
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void UpdatePerception(float DeltaTime);
    
    // Observation queries
    UFUNCTION(BlueprintCallable, Category = "Perception")
    AActor* GetPrimaryFocusTarget() const { return PrimaryFocusTarget; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    float GetPrimaryFocusIntensity() const { return PrimaryFocusIntensity; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    TArray<AActor*> GetPeripheralObjects() const { return PeripheralObjects; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    float GetObservationDuration(AActor* InActor) const;
    
    // Anomaly detection
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void RegisterAnomaly(const FVector& AnomalyLocation, float AnomalySeverity);
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    bool HasPlayerNoticedAnomaly() const { return bHasNoticedAnomaly; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    float GetAnomalyAwareness() const { return AnomalyAwareness; }
    
    // Attention mechanics
    UFUNCTION(BlueprintCallable, Category = "Perception")
    float GetAttentionCapacity() const { return AttentionCapacity; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    float GetCurrentAttentionLoad() const { return CurrentAttentionLoad; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void ApplyAttentionStress(float StressAmount);
    
    // Sanity/psychological effects
    UFUNCTION(BlueprintCallable, Category = "Perception")
    float GetSanity() const { return CurrentSanity; }
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void ReduceSanity(float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void RestoreSanity(float Amount);
    
    // Observation mode
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void EnterIntenseObservationMode();
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    void ExitIntenseObservationMode();
    
    UFUNCTION(BlueprintCallable, Category = "Perception")
    bool IsInIntenseObservationMode() const { return bIntenseObservationMode; }
    
protected:
    // Camera state
    FVector CameraLocation;
    FRotator CameraRotation;
    FVector CameraForward;
    FVector CameraRight;
    FVector CameraUp;
    
    // Primary observation target
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    AActor* PrimaryFocusTarget;
    
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    float PrimaryFocusIntensity;
    
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    float PrimaryFocusDuration;
    
    // Peripheral vision
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    TArray<AActor*> PeripheralObjects;
    
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    TMap<AActor*, FObservedObject> ObservedObjects;
    
    // Anomaly system
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    bool bHasNoticedAnomaly;
    
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    float AnomalyAwareness;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float AnomalyDetectionThreshold;
    
    // Attention system
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    float AttentionCapacity;
    
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    float CurrentAttentionLoad;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float MaxAttentionCapacity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float AttentionRecoveryRate;
    
    // Sanity system
    UPROPERTY(BlueprintReadOnly, Category = "Perception")
    float CurrentSanity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float MaxSanity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float SanityRecoveryRate;
    
    // Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float FieldOfViewAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float MaxPerceptionDistance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    float PeripheralVisionAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
    bool bIntenseObservationMode;
    
    virtual void PerformRaycastPerception();
    virtual void UpdateFocusTarget();
    virtual void UpdatePeripheralVision();
    virtual void ProcessObservedObjects(float DeltaTime);
    virtual void UpdateAnomalyDetection();
    virtual void UpdateAttentionSystem(float DeltaTime);
    virtual void UpdateSanitySystem(float DeltaTime);
    
private:
    FPerceptionRay CreatePerceptionRay(const FVector& Direction);
    EPeripheralVision CalculatePeripheralType(const FVector& ObjectLocation);
};
