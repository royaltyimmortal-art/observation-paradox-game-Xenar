#ifndef OBSERVABLE_ACTOR_H
#define OBSERVABLE_ACTOR_H

#include "ObservationParadox.h"

namespace ObservationParadox {

/**
 * Base class for any actor that participates in quantum observation mechanics
 */
class AObservableActor {
public:
    AObservableActor();
    virtual ~AObservableActor();
    
    virtual void BeginPlay();
    virtual void EndPlay();
    virtual void Tick(float DeltaTime);
    
    // Observation mechanics
    virtual void OnObserved();
    virtual void OnUnobserved();
    virtual void OnObservationLost();
    
    // Quantum state management
    void RegisterQuantumStates(const std::vector<FQuantumState>& InStates);
    FQuantumState GetCurrentQuantumState() const { return CurrentState; }
    void ApplyQuantumState(const FQuantumState& NewState);
    
    // Reality zone management
    void SetRealityZone(ERealityZone NewZone) { CurrentRealityZone = NewZone; }
    ERealityZone GetRealityZone() const { return CurrentRealityZone; }
    
    // Observation tracking
    EObservationState GetObservationState() const;
    float GetObservationDuration() const { return CurrentObservationDuration; }
    float GetTimeSinceLastObserved() const { return TimeSinceLastObserved; }
    
    // Distortion effects
    void TriggerGlitch(float Intensity, float Duration);
    void ShiftToRandomState();
    void EnterFractureState();
    
    // Accessors
    FVector GetActorLocation() const { return Location; }
    void SetActorLocation(const FVector& NewLocation) { Location = NewLocation; }
    
    FRotator GetActorRotation() const { return Rotation; }
    void SetActorRotation(const FRotator& NewRotation) { Rotation = NewRotation; }
    
    FVector GetActorScale() const { return Scale; }
    void SetActorScale(const FVector& NewScale) { Scale = NewScale; }
    
    bool IsHidden() const { return bHidden; }
    void SetHidden(bool bInHidden) { bHidden = bInHidden; }
    
    virtual void Print() const;
    
protected:
    std::vector<FQuantumState> PossibleStates;
    FQuantumState CurrentState;
    EObservationState ObservationState;
    ERealityZone CurrentRealityZone;
    
    float ObservationDecayTime;
    float CurrentObservationDuration;
    float TimeSinceLastObserved;
    
    float CurrentGlitchIntensity;
    float GlitchDuration;
    
    bool bCanBeUnobserved;
    bool bIsFracturable;
    float UnobservedMovementRadius;
    float StateTransitionSpeed;
    
    FVector Location;
    FRotator Rotation;
    FVector Scale;
    
    FVector StoredLocation;
    FRotator StoredRotation;
    FVector StoredScale;
    
    bool bHidden;
    
    virtual void UpdateQuantumState(float DeltaTime);
    virtual void ApplyGlitchVisuals(float DeltaTime);
    virtual void HandleObservationDecay(float DeltaTime);
    
private:
    void RegisterWithSystem();
    void UnregisterFromSystem();
};

}  // namespace ObservationParadox

#endif // OBSERVABLE_ACTOR_H
