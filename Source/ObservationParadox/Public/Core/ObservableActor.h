#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuantumObservationSystem.h"
#include "ObservableActor.generated.h"

/**
 * Base class for any actor that can participate in quantum observation mechanics
 * 
 * These actors can:
 * - Change position/rotation when unobserved
 * - Shift between multiple possible states
 * - Respond to player observation
 * - Trigger reality distortions
 * - Remember observation history
 */
UCLASS()
class OBSERVATIONPARADOX_API AObservableActor : public AActor
{
    GENERATED_BODY()

public:
    AObservableActor();
    
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaTime) override;
    
    // Observation mechanics
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void OnObserved();
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void OnUnobserved();
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void OnObservationLost();
    
    // Quantum state management
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void RegisterQuantumStates(const TArray<FQuantumState>& InStates);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    FQuantumState GetCurrentQuantumState() const;
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void ApplyQuantumState(const FQuantumState& NewState);
    
    // Reality zone management
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void SetRealityZone(ERealityZone NewZone);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    ERealityZone GetRealityZone() const { return CurrentRealityZone; }
    
    // Observation tracking
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    EObservationState GetObservationState() const;
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    float GetObservationDuration() const { return CurrentObservationDuration; }
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    float GetTimeSinceLastObserved() const { return TimeSinceLastObserved; }
    
    // Distortion effects
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void TriggerGlitch(float Intensity, float Duration);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void ShiftToRandomState();
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void EnterFractureState();
    
    // NPC-specific interaction
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quantum")
    void OnBeingWatched();
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quantum")
    void OnNoLongerWatched();
    
protected:
    // Quantum state storage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    TArray<FQuantumState> PossibleStates;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    FQuantumState CurrentState;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    EObservationState ObservationState;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    ERealityZone CurrentRealityZone;
    
    // Observation tracking
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    float ObservationDecayTime;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    float CurrentObservationDuration;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    float TimeSinceLastObserved;
    
    // Glitch and distortion
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    float CurrentGlitchIntensity;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    float GlitchDuration;
    
    // Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    bool bCanBeUnobserved;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    bool bIsFracturable;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    float UnobservedMovementRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    float StateTransitionSpeed;
    
    // Stored data for reality restoration
    FVector StoredLocation;
    FRotator StoredRotation;
    FVector StoredScale;
    
    virtual void UpdateQuantumState(float DeltaTime);
    virtual void ApplyGlitchVisuals(float DeltaTime);
    virtual void HandleObservationDecay(float DeltaTime);
    
private:
    void RegisterWithSystem();
    void UnregisterFromSystem();
};
