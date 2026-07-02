#ifndef REALITY_MANAGER_H
#define REALITY_MANAGER_H

#include "ObservationParadox.h"

namespace ObservationParadox {

/**
 * REALITY MANAGER
 * Manages different reality zones and transitions
 */
class URealityManager {
public:
    URealityManager();
    ~URealityManager();
    
    void Initialize();
    void Deinitialize();
    void Update(float DeltaTime);
    
    // Zone management
    void CreateRealityZone(const FVector& Center, float Radius, ERealityZone ZoneType);
    void RemoveRealityZone(FVector ZoneCenter);
    void UpdateZoneCorruption(FVector ZoneCenter, float CorruptionDelta);
    FRealityZoneData GetZoneAtLocation(const FVector& Location);
    
    // Zone transition
    void TransitionToZone(AObservableActor* InActor, ERealityZone TargetZone);
    
    // Environmental effects
    void ApplyZoneEffects(const FVector& Location, float DeltaTime);
    void SpreadCorruption(const FVector& CorruptionPoint, float CorruptionAmount);
    
    // Reality queries
    ERealityZone GetRealityZoneAtLocation(const FVector& Location);
    float GetTimeMultiplierAtLocation(const FVector& Location);
    float GetCorruptionLevelAtLocation(const FVector& Location);
    
    // Global reality effects
    void TriggerGlobalFracture(float Intensity);
    void InitiateParadoxLoop();
    void RestabilizeReality();
    
    // Singleton access
    static URealityManager& Get();
    
private:
    std::vector<FRealityZoneData> ActiveZones;
    
    float GlobalCorruptionLevel;
    float GlobalTimeScale;
    
    float CorruptionSpreadRate;
    float MaxCorruptionLevel;
    float ZoneTransitionSpeed;
    
    void UpdateAllZones(float DeltaTime);
    void CheckAndApplyZoneTransitions();
    void UpdateCorruptionEffects(float DeltaTime);
    void ProcessEnvironmentalChanges(float DeltaTime);
};

}  // namespace ObservationParadox

#endif // REALITY_MANAGER_H
