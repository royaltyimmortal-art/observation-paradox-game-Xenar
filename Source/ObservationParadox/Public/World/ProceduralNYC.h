#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ProceduralNYC.generated.h"

/**
 * PROCEDURAL NYC GENERATION SYSTEM
 * 
 * Manages dynamic generation and mutation of NYC:
 * - Street layout changes when unobserved
 * - Building positions shift
 * - Districts can rearrange
 * - Quantum procedural algorithm ensures consistency when observed
 * 
 * Key Features:
 * - Seeded procedural generation for reproducibility
 * - Observation-based locking of generated content
 * - Efficient culling and streaming
 * - Real-time generation with performance optimization
 */

STRUCT_TYPE(FBuildingData)
{
    GENERATED_BODY()

    FVector BaseLocation;
    FVector CurrentLocation;
    FVector Scale;
    FRotator Rotation;
    int32 BuildingType;  // 0: Residential, 1: Commercial, 2: Office, 3: Industrial
    int32 Floors;
    FString ModelID;
    bool bIsGenerated;
    bool bIsObserved;
    uint32 LocationHash;  // Deterministic variation seed
};

STRUCT_TYPE(FStreetSegment)
{
    GENERATED_BODY()

    FVector StartPoint;
    FVector EndPoint;
    FVector CurrentEndPoint;  // Can shift when unobserved
    float Width;
    bool bIsObserved;
    TArray<FVector> Intersections;
    uint32 GeometryHash;
};

STRUCT_TYPE(FNYCDistrict)
{
    GENERATED_BODY()

    FString DistrictName;
    FVector DistrictCenter;
    float DistrictRadius;
    TArray<FBuildingData> Buildings;
    TArray<FStreetSegment> Streets;
    ERealityZone ZoneType;
    float CorruptionLevel;
    bool bIsGenerated;
};

UCLASS()
class OBSERVATIONPARADOX_API UProceduralNYC : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UProceduralNYC();
    
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    
    // Generation
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void GenerateDistrict(const FString& DistrictName, const FVector& Center, float Radius);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void GenerateStreetGrid(const FVector& CenterLocation, float GridSize, int32 GridDimensions);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void PopulateWithBuildings(const FVector& Area, float AreaRadius, int32 BuildingCount);
    
    // Mutation (when unobserved)
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void MutateDistrictLayout(const FString& DistrictName);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void ShiftStreets(const FVector& CenterLocation, float DisplacementAmount);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void RelocateBuildings(const FVector& CenterLocation, float MaxDistance);
    
    // Observation locking
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void LockGeometryAtLocation(const FVector& Location, float LockRadius);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void UnlockGeometryAtLocation(const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    bool IsGeometryLockedAtLocation(const FVector& Location);
    
    // Querying
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    FNYCDistrict GetDistrict(const FString& DistrictName);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    TArray<FBuildingData> GetBuildingsInRadius(const FVector& Location, float Radius);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    FStreetSegment GetNearestStreet(const FVector& Location);
    
    // Advanced generation
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void CreateFractureZoneGeometry(const FVector& CenterLocation, float Radius);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void CreateNullZoneGeometry(const FVector& CenterLocation, float Radius);
    
    // Streaming
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void StreamInRegion(const FVector& RegionCenter, float StreamRadius);
    
    UFUNCTION(BlueprintCallable, Category = "ProceduralNYC")
    void StreamOutRegion(const FVector& RegionCenter);
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "ProceduralNYC")
    TMap<FString, FNYCDistrict> Districts;
    
    UPROPERTY(BlueprintReadOnly, Category = "ProceduralNYC")
    TMap<FVector, bool> LockedGeometryLocations;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralNYC")
    int32 ProceduralSeed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralNYC")
    float MutationRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralNYC")
    float MaxStreetShift;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralNYC")
    float MaxBuildingRelocate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralNYC")
    int32 MaxBuildingsPerDistrict;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralNYC")
    float LockDuration;
    
    virtual void UpdateMutations(float DeltaTime);
    virtual void ProcessLockedGeometry();
    
private:
    FVector GenerateRandomBuildingLocation(const FVector& CenterLocation, float Radius);
    FBuildingData CreateBuilding(const FVector& Location, int32 BuildingType);
    uint32 GenerateDeterministicHash(const FVector& Location);
};
