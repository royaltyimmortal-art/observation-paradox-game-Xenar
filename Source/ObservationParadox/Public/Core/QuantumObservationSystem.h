#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Containers/List.h"

/**
 * QUANTUM OBSERVATION SYSTEM - Core Game Mechanic
 * 
 * This system manages the fundamental reality distortion mechanic:
 * - When the player OBSERVES something, reality locks into one state
 * - When the player LOOKS AWAY, reality changes silently
 * - The world is never consistent twice
 * 
 * Key Components:
 * 1. Observation Tracking - What is the player looking at?
 * 2. Reality State Management - Multiple possible states for each actor
 * 3. Collapse Mechanics - Lock reality when observed
 * 4. Uncertainty Engine - Generate variations when unobserved
 */

// Represents different states an observable object can exist in
ENUM(uint8, EObservationState)
{
    EOS_Unobserved,      // Reality is fluid, undefined
    EOS_Collapsing,      // Transitioning to observed state
    EOS_Observed,        // Reality is locked
    EOS_Uncertainty,     // In quantum flux, possible states exist
    EOS_Fractured,       // Reality is breaking down
    EOS_Null             // Non-euclidean, liminal space
};

// Reality zones with different physics
ENUM(uint8, ERealityZone)
{
    ERZ_Stable,          // Normal NYC physics
    ERZ_Fracture,        // Reality glitches, shifting geometry
    ERZ_Null,            // Backrooms-style infinite spaces
    ERZ_Threshold,       // Transitional areas between zones
    ERZ_Mirrored         // Reflected/duplicate realities
};

// Data structure for a possible reality state
STRUCT_TYPE(FQuantumState)
{
    GENERATED_BODY()

    FVector Location;
    FRotator Rotation;
    FVector Scale;
    bool bVisible;
    bool bPhysicsEnabled;
    uint8 VariationID;
    float Probability;  // Likelihood this state manifests
    
    FString Description;
    uint8 StateHash;    // For quick comparison
};

// Observation data for a single frame
STRUCT_TYPE(FObservationData)
{
    GENERATED_BODY()

    AActor* ObservedActor;
    FVector ObservationDirection;
    float ObservationDistance;
    float FocusIntensity;  // 0.0 to 1.0, how much attention is on this
    float ObservationDuration;  // How long has this been observed?
    bool bIsDirect;  // Direct look vs peripheral vision
};

class AObservableActor;

/**
 * Central management system for quantum mechanics in the game
 * Singleton pattern - only one instance manages all observations
 */
class OBSERVATIONPARADOX_API FQuantumObservationSystem
{
public:
    FQuantumObservationSystem();
    ~FQuantumObservationSystem();
    
    // Initialize the system
    void Initialize();
    void Shutdown();
    
    // Main update loop - called every frame
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
    TArray<FQuantumState> GenerateMultipleStates(AObservableActor* InActor, int32 Count);
    
    // Reality distortion effects
    void ApplyGlitchEffect(AObservableActor* InActor, float Intensity);
    void ShiftGeometry(AObservableActor* InActor, const FVector& TargetLocation);
    void CreateFractureZone(const FVector& CenterLocation, float Radius);
    
    // Time and causality
    void DesynchronizeTime(AObservableActor* InActor, float TimeOffset);
    float GetLocalTimeMultiplier(const FVector& Location) const;
    
    // Utility
    static FQuantumObservationSystem& Get();
    
private:
    // Observation tracking
    TArray<TWeakObjectPtr<AObservableActor>> RegisteredObservables;
    TMap<TWeakObjectPtr<AObservableActor>, EObservationState> ObservationStates;
    TMap<TWeakObjectPtr<AObservableActor>, FObservationData> CurrentObservations;
    TMap<TWeakObjectPtr<AObservableActor>, TArray<FQuantumState>> PossibleStates;
    
    // Reality management
    TMap<FVector, ERealityZone> RealityZoneMap;
    TArray<FVector> FractureZones;
    
    // Time and causality
    float GlobalTimeScale;
    TMap<AObservableActor*, float> LocalTimeOffsets;
    
    // Performance and optimization
    float LastUpdateTime;
    int32 MaxObservableActors;
    float ObservationDecayTime;  // How long until observation effect wears off
    
    // Private helper functions
    void UpdateObservationStates(float DeltaTime);
    void ProcessUncertaintyDecay(float DeltaTime);
    void ApplyQuantumEffects(float DeltaTime);
    uint8 CalculateStateHash(const FQuantumState& State) const;
    FVector CalculateUncertaintyVariation(const FVector& BaseLocation);
};
