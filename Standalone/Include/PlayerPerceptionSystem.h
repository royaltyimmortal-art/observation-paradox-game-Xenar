#ifndef PLAYER_PERCEPTION_SYSTEM_H
#define PLAYER_PERCEPTION_SYSTEM_H

#include "ObservationParadox.h"

namespace ObservationParadox {

enum class EPeripheralVision : uint8_t {
    DirectFocus,    // Looking directly at something
    Peripheral,     // In edge of vision
    Barely,         // Barely noticeable
    NotVisible      // Not in field of view
};

struct FObservedObject {
    AObservableActor* ObjectActor;
    float ObservationDuration;
    float FocusIntensity;
    FVector LastSeenLocation;
    bool bHasBeenSeen;
    float TimeSinceLastSeen;
};

/**
 * PLAYER PERCEPTION SYSTEM
 * Tracks what the player is observing
 */
class UPlayerPerceptionSystem {
public:
    UPlayerPerceptionSystem();
    ~UPlayerPerceptionSystem();
    
    void Initialize();
    void Deinitialize();
    void Update(float DeltaTime);
    
    // Camera/viewing setup
    void SetViewport(const FVector& CameraLocation, const FRotator& CameraRotation);
    
    // Main perception update
    void UpdatePerception(float DeltaTime);
    
    // Observation queries
    AObservableActor* GetPrimaryFocusTarget() const { return PrimaryFocusTarget; }
    float GetPrimaryFocusIntensity() const { return PrimaryFocusIntensity; }
    std::vector<AObservableActor*> GetPeripheralObjects() const { return PeripheralObjects; }
    float GetObservationDuration(AObservableActor* InActor) const;
    
    // Anomaly detection
    void RegisterAnomaly(const FVector& AnomalyLocation, float AnomalySeverity);
    bool HasPlayerNoticedAnomaly() const { return bHasNoticedAnomaly; }
    float GetAnomalyAwareness() const { return AnomalyAwareness; }
    
    // Attention mechanics
    float GetAttentionCapacity() const { return AttentionCapacity; }
    float GetCurrentAttentionLoad() const { return CurrentAttentionLoad; }
    void ApplyAttentionStress(float StressAmount);
    
    // Sanity/psychological effects
    float GetSanity() const { return CurrentSanity; }
    void ReduceSanity(float Amount);
    void RestoreSanity(float Amount);
    
    // Observation mode
    void EnterIntenseObservationMode();
    void ExitIntenseObservationMode();
    bool IsInIntenseObservationMode() const { return bIntenseObservationMode; }
    
    // Singleton access
    static UPlayerPerceptionSystem& Get();
    
private:
    FVector CameraLocation;
    FRotator CameraRotation;
    FVector CameraForward;
    FVector CameraRight;
    FVector CameraUp;
    
    AObservableActor* PrimaryFocusTarget;
    float PrimaryFocusIntensity;
    float PrimaryFocusDuration;
    
    std::vector<AObservableActor*> PeripheralObjects;
    std::map<AObservableActor*, FObservedObject> ObservedObjects;
    
    bool bHasNoticedAnomaly;
    float AnomalyAwareness;
    float AnomalyDetectionThreshold;
    
    float AttentionCapacity;
    float CurrentAttentionLoad;
    float MaxAttentionCapacity;
    float AttentionRecoveryRate;
    
    float CurrentSanity;
    float MaxSanity;
    float SanityRecoveryRate;
    
    float FieldOfViewAngle;
    float MaxPerceptionDistance;
    float PeripheralVisionAngle;
    bool bIntenseObservationMode;
    
    void PerformRaycastPerception();
    void UpdateFocusTarget();
    void UpdatePeripheralVision();
    void ProcessObservedObjects(float DeltaTime);
    void UpdateAnomalyDetection();
    void UpdateAttentionSystem(float DeltaTime);
    void UpdateSanitySystem(float DeltaTime);
    
    EPeripheralVision CalculatePeripheralType(const FVector& ObjectLocation);
};

}  // namespace ObservationParadox

#endif // PLAYER_PERCEPTION_SYSTEM_H
