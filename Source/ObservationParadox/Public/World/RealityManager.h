#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/QuantumObservationSystem.h"
#include "RealityManager.generated.h"

/**
 * REALITY MANAGER
 * 
 * Manages the different reality zones and transitions:
 * - STABLE ZONES: Normal NYC physics and physics
 * - FRACTURE ZONES: Reality glitches, shifting geometry, time distortions
 * - NULL ZONES: Backrooms-style liminal spaces, non-euclidean geometry
 * - THRESHOLD ZONES: Transitional areas between zones
 * - MIRRORED ZONES: Reflected realities and duplicate spaces
 * 
 * Handles:
 * - Zone transitions and physics changes
 * - Environmental state management
 * - Reality corruption and spreading
 * - Zone-specific effects and rules
 */

STRUCT_TYPE(FRealityZoneData)
{
    GENERATED_BODY()

    FVector ZoneCenter;
    float ZoneRadius;
    ERealityZone ZoneType;
    float TimeMultiplier;  // Time flows differently here
    float CorruptionLevel;  // 0.0 (stable) to 1.0 (completely fractured)
    bool bIsActive;
    TArray<AActor*> AffectedActors;
    float LastUpdateTime;
};

UCLASS()
class OBSERVATIONPARADOX_API URealityManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    URealityManager();
    
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    
    // Zone management
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void CreateRealityZone(const FVector& Center, float Radius, ERealityZone ZoneType);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void RemoveRealityZone(FVector ZoneCenter);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void UpdateZoneCorruption(FVector ZoneCenter, float CorruptionDelta);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    FRealityZoneData GetZoneAtLocation(const FVector& Location);
    
    // Zone transition
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void TransitionToZone(AActor* InActor, ERealityZone TargetZone);
    
    // Environmental effects
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void ApplyZoneEffects(const FVector& Location, float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void SpreadCorruption(const FVector& CorruptionPoint, float CorruptionAmount);
    
    // Reality queries
    UFUNCTION(BlueprintCallable, Category = "Reality")
    ERealityZone GetRealityZoneAtLocation(const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    float GetTimeMultiplierAtLocation(const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    float GetCorruptionLevelAtLocation(const FVector& Location);
    
    // Global reality effects
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void TriggerGlobalFracture(float Intensity);
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void InitiateParadoxLoop();
    
    UFUNCTION(BlueprintCallable, Category = "Reality")
    void RestabilizeReality();
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "Reality")
    TArray<FRealityZoneData> ActiveZones;
    
    UPROPERTY(BlueprintReadOnly, Category = "Reality")
    float GlobalCorruptionLevel;
    
    UPROPERTY(BlueprintReadOnly, Category = "Reality")
    float GlobalTimeScale;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reality")
    float CorruptionSpreadRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reality")
    float MaxCorruptionLevel;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reality")
    float ZoneTransitionSpeed;
    
    virtual void UpdateAllZones(float DeltaTime);
    virtual void CheckAndApplyZoneTransitions();
    virtual void UpdateCorruptionEffects(float DeltaTime);
    virtual void ProcessEnvironmentalChanges(float DeltaTime);
};
