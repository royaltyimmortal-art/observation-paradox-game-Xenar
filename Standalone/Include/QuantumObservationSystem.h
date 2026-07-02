#ifndef QUANTUM_OBSERVATION_SYSTEM_H
#define QUANTUM_OBSERVATION_SYSTEM_H

#include "ObservationParadox.h"

namespace ObservationParadox {

/**
 * QUANTUM OBSERVATION SYSTEM - Core Game Mechanic
 * Manages observation-based reality manipulation
 */
class FQuantumObservationSystem {
public:
    FQuantumObservationSystem();
    ~FQuantumObservationSystem();
    
    void Initialize();
    void Shutdown();
    void Update(float DeltaTime);
    
    // Register/unregister observable actors
    void RegisterObservable(AObservableActor* InActor);
    void UnregisterObservable(AObservableActor* InActor);
    
    // Record what the player is observing
    void SetPlayerObservation(const FObservationData& ObservationData);
    void ClearPlayerObservation();
    
    // Core observation mechanics
    void CollapseReality(AObservableActor* InActor);
    void UncertaintyFlux(AObservableActor* InActor);
    void TriggerFracture(AObservableActor* InActor);
    
    // Query observation state
    EObservationState GetObservationState(AObservableActor* InActor) const;
    ERealityZone GetRealityZone(const FVector& Location) const;
    
    // Generate new quantum states
    FQuantumState GenerateUncertaintyState(AObservableActor* InActor);
    std::vector<FQuantumState> GenerateMultipleStates(AObservableActor* InActor, int Count);
    
    // Reality distortion effects
    void ApplyGlitchEffect(AObservableActor* InActor, float Intensity);
    void ShiftGeometry(AObservableActor* InActor, const FVector& TargetLocation);
    void CreateFractureZone(const FVector& CenterLocation, float Radius);
    
    // Time and causality
    void DesynchronizeTime(AObservableActor* InActor, float TimeOffset);
    float GetLocalTimeMultiplier(const FVector& Location) const;
    
    // Singleton access
    static FQuantumObservationSystem& Get();
    
private:
    std::vector<AObservableActor*> RegisteredObservables;
    std::map<AObservableActor*, EObservationState> ObservationStates;
    std::map<AObservableActor*, FObservationData> CurrentObservations;
    std::map<AObservableActor*, std::vector<FQuantumState>> PossibleStates;
    
    std::map<FVector, ERealityZone> RealityZoneMap;
    std::vector<FVector> FractureZones;
    
    float GlobalTimeScale;
    std::map<AObservableActor*, float> LocalTimeOffsets;
    
    float LastUpdateTime;
    int MaxObservableActors;
    float ObservationDecayTime;
    
    void UpdateObservationStates(float DeltaTime);
    void ProcessUncertaintyDecay(float DeltaTime);
    void ApplyQuantumEffects(float DeltaTime);
    uint8_t CalculateStateHash(const FQuantumState& State) const;
    FVector CalculateUncertaintyVariation(const FVector& BaseLocation);
};

}  // namespace ObservationParadox

#endif // QUANTUM_OBSERVATION_SYSTEM_H
