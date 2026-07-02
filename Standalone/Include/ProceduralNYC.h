#ifndef PROCEDURAL_NYC_H
#define PROCEDURAL_NYC_H

#include "ObservationParadox.h"

namespace ObservationParadox {

struct FStreetSegment {
    FVector StartPoint;
    FVector EndPoint;
    FVector CurrentEndPoint;
    float Width;
    bool bIsObserved;
    std::vector<FVector> Intersections;
    uint32_t GeometryHash;
};

struct FNYCDistrict {
    std::string DistrictName;
    FVector DistrictCenter;
    float DistrictRadius;
    std::vector<FBuildingData> Buildings;
    std::vector<FStreetSegment> Streets;
    ERealityZone ZoneType;
    float CorruptionLevel;
    bool bIsGenerated;
};

/**
 * PROCEDURAL NYC GENERATION SYSTEM
 * Manages dynamic generation and mutation of NYC
 */
class UProceduralNYC {
public:
    UProceduralNYC();
    ~UProceduralNYC();
    
    void Initialize();
    void Deinitialize();
    void Update(float DeltaTime);
    
    // Generation
    void GenerateDistrict(const std::string& DistrictName, const FVector& Center, float Radius);
    void GenerateStreetGrid(const FVector& CenterLocation, float GridSize, int GridDimensions);
    void PopulateWithBuildings(const FVector& Area, float AreaRadius, int BuildingCount);
    
    // Mutation (when unobserved)
    void MutateDistrictLayout(const std::string& DistrictName);
    void ShiftStreets(const FVector& CenterLocation, float DisplacementAmount);
    void RelocateBuildings(const FVector& CenterLocation, float MaxDistance);
    
    // Observation locking
    void LockGeometryAtLocation(const FVector& Location, float LockRadius);
    void UnlockGeometryAtLocation(const FVector& Location);
    bool IsGeometryLockedAtLocation(const FVector& Location);
    
    // Querying
    FNYCDistrict GetDistrict(const std::string& DistrictName);
    std::vector<FBuildingData> GetBuildingsInRadius(const FVector& Location, float Radius);
    FStreetSegment GetNearestStreet(const FVector& Location);
    
    // Advanced generation
    void CreateFractureZoneGeometry(const FVector& CenterLocation, float Radius);
    void CreateNullZoneGeometry(const FVector& CenterLocation, float Radius);
    
    // Streaming
    void StreamInRegion(const FVector& RegionCenter, float StreamRadius);
    void StreamOutRegion(const FVector& RegionCenter);
    
    // Singleton access
    static UProceduralNYC& Get();
    
private:
    std::map<std::string, FNYCDistrict> Districts;
    std::map<FVector, bool> LockedGeometryLocations;
    
    int ProceduralSeed;
    float MutationRate;
    float MaxStreetShift;
    float MaxBuildingRelocate;
    int MaxBuildingsPerDistrict;
    float LockDuration;
    
    void UpdateMutations(float DeltaTime);
    void ProcessLockedGeometry();
    
    FVector GenerateRandomBuildingLocation(const FVector& CenterLocation, float Radius);
    FBuildingData CreateBuilding(const FVector& Location, int BuildingType);
    uint32_t GenerateDeterministicHash(const FVector& Location);
};

}  // namespace ObservationParadox

#endif // PROCEDURAL_NYC_H
