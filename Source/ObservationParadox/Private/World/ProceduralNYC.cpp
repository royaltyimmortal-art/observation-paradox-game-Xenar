#include "World/ProceduralNYC.h"
#include "Math/RandomStream.h"

UProceduralNYC::UProceduralNYC()
    : ProceduralSeed(12345)
    , MutationRate(0.3f)
    , MaxStreetShift(500.0f)
    , MaxBuildingRelocate(1000.0f)
    , MaxBuildingsPerDistrict(50)
    , LockDuration(5.0f)
{
}

void UProceduralNYC::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Warning, TEXT("Procedural NYC Initialized with seed: %d"), ProceduralSeed);
}

void UProceduralNYC::Deinitialize()
{
    Districts.Empty();
    LockedGeometryLocations.Empty();
    Super::Deinitialize();
}

void UProceduralNYC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateMutations(DeltaTime);
    ProcessLockedGeometry();
}

void UProceduralNYC::GenerateDistrict(const FString& DistrictName, const FVector& Center, float Radius)
{
    FNYCDistrict NewDistrict;
    NewDistrict.DistrictName = DistrictName;
    NewDistrict.DistrictCenter = Center;
    NewDistrict.DistrictRadius = Radius;
    NewDistrict.ZoneType = ERZ_Stable;
    NewDistrict.CorruptionLevel = 0.0f;
    NewDistrict.bIsGenerated = true;
    
    // Generate street grid
    GenerateStreetGrid(Center, Radius * 2.0f, 5);
    
    // Populate with buildings
    PopulateWithBuildings(Center, Radius, MaxBuildingsPerDistrict);
    
    Districts.Add(DistrictName, NewDistrict);
    
    UE_LOG(LogTemp, Warning, TEXT("Generated district: %s"), *DistrictName);
}

void UProceduralNYC::GenerateStreetGrid(const FVector& CenterLocation, float GridSize, int32 GridDimensions)
{
    FNYCDistrict* DistrictPtr = nullptr;
    
    // Find nearest district
    for (auto& DistrictPair : Districts)
    {
        if (FVector::Dist(DistrictPair.Value.DistrictCenter, CenterLocation) < 5000.0f)
        {
            DistrictPtr = &DistrictPair.Value;
            break;
        }
    }
    
    if (!DistrictPtr)
    {
        return;
    }
    
    float StreetSpacing = GridSize / GridDimensions;
    
    // Generate horizontal streets
    for (int32 y = 0; y < GridDimensions; ++y)
    {
        FStreetSegment Street;
        Street.StartPoint = CenterLocation + FVector(-GridSize * 0.5f, -GridSize * 0.5f + y * StreetSpacing, 0.0f);
        Street.EndPoint = CenterLocation + FVector(GridSize * 0.5f, -GridSize * 0.5f + y * StreetSpacing, 0.0f);
        Street.CurrentEndPoint = Street.EndPoint;
        Street.Width = 25.0f;  // Street width in meters
        Street.bIsObserved = false;
        Street.GeometryHash = GenerateDeterministicHash(Street.StartPoint);
        
        DistrictPtr->Streets.Add(Street);
    }
    
    // Generate vertical streets
    for (int32 x = 0; x < GridDimensions; ++x)
    {
        FStreetSegment Street;
        Street.StartPoint = CenterLocation + FVector(-GridSize * 0.5f + x * StreetSpacing, -GridSize * 0.5f, 0.0f);
        Street.EndPoint = CenterLocation + FVector(-GridSize * 0.5f + x * StreetSpacing, GridSize * 0.5f, 0.0f);
        Street.CurrentEndPoint = Street.EndPoint;
        Street.Width = 25.0f;
        Street.bIsObserved = false;
        Street.GeometryHash = GenerateDeterministicHash(Street.StartPoint);
        
        DistrictPtr->Streets.Add(Street);
    }
}

void UProceduralNYC::PopulateWithBuildings(const FVector& Area, float AreaRadius, int32 BuildingCount)
{
    FNYCDistrict* DistrictPtr = nullptr;
    
    // Find district at this location
    for (auto& DistrictPair : Districts)
    {
        if (FVector::Dist(DistrictPair.Value.DistrictCenter, Area) < AreaRadius)
        {
            DistrictPtr = &DistrictPair.Value;
            break;
        }
    }
    
    if (!DistrictPtr)
    {
        return;
    }
    
    // Place buildings
    for (int32 i = 0; i < BuildingCount; ++i)
    {
        FVector BuildingLocation = GenerateRandomBuildingLocation(Area, AreaRadius);
        int32 BuildingType = FMath::Rand() % 4;  // 0-3: Residential, Commercial, Office, Industrial
        
        FBuildingData Building = CreateBuilding(BuildingLocation, BuildingType);
        DistrictPtr->Buildings.Add(Building);
    }
}

void UProceduralNYC::MutateDistrictLayout(const FString& DistrictName)
{
    FNYCDistrict* DistrictPtr = Districts.Find(DistrictName);
    if (!DistrictPtr)
    {
        return;
    }
    
    // Shift streets
    for (FStreetSegment& Street : DistrictPtr->Streets)
    {
        if (!Street.bIsObserved && FMath::FRand() < MutationRate)
        {
            FVector Shift;
            Shift.X = FMath::FRandRange(-MaxStreetShift, MaxStreetShift);
            Shift.Y = FMath::FRandRange(-MaxStreetShift, MaxStreetShift);
            Shift.Z = 0.0f;
            
            Street.CurrentEndPoint = Street.EndPoint + Shift;
        }
    }
    
    // Relocate buildings
    for (FBuildingData& Building : DistrictPtr->Buildings)
    {
        if (!Building.bIsObserved && FMath::FRand() < MutationRate)
        {
            Building.CurrentLocation = GenerateRandomBuildingLocation(DistrictPtr->DistrictCenter, DistrictPtr->DistrictRadius);
        }
    }
}

void UProceduralNYC::ShiftStreets(const FVector& CenterLocation, float DisplacementAmount)
{
    for (auto& DistrictPair : Districts)
    {
        for (FStreetSegment& Street : DistrictPair.Value.Streets)
        {
            if (!Street.bIsObserved)
            {
                float Distance = FVector::Dist((Street.StartPoint + Street.EndPoint) * 0.5f, CenterLocation);
                if (Distance < 2000.0f)  // Only affect nearby streets
                {
                    FVector Direction = ((Street.StartPoint + Street.EndPoint) * 0.5f - CenterLocation).GetSafeNormal();
                    Street.CurrentEndPoint += Direction * DisplacementAmount;
                }
            }
        }
    }
}

void UProceduralNYC::RelocateBuildings(const FVector& CenterLocation, float MaxDistance)
{
    for (auto& DistrictPair : Districts)
    {
        for (FBuildingData& Building : DistrictPair.Value.Buildings)
        {
            if (!Building.bIsObserved)
            {
                float Distance = FVector::Dist(Building.BaseLocation, CenterLocation);
                if (Distance < MaxDistance)
                {
                    Building.CurrentLocation = GenerateRandomBuildingLocation(CenterLocation, MaxDistance);
                }
            }
        }
    }
}

void UProceduralNYC::LockGeometryAtLocation(const FVector& Location, float LockRadius)
{
    LockedGeometryLocations.Add(Location, true);
    
    // Lock all geometry in this radius
    for (auto& DistrictPair : Districts)
    {
        for (FStreetSegment& Street : DistrictPair.Value.Streets)
        {
            if (FVector::Dist((Street.StartPoint + Street.EndPoint) * 0.5f, Location) < LockRadius)
            {
                Street.bIsObserved = true;
            }
        }
        
        for (FBuildingData& Building : DistrictPair.Value.Buildings)
        {
            if (FVector::Dist(Building.BaseLocation, Location) < LockRadius)
            {
                Building.bIsObserved = true;
            }
        }
    }
}

void UProceduralNYC::UnlockGeometryAtLocation(const FVector& Location)
{
    LockedGeometryLocations.Remove(Location);
}

bool UProceduralNYC::IsGeometryLockedAtLocation(const FVector& Location)
{
    return LockedGeometryLocations.Contains(Location);
}

FNYCDistrict UProceduralNYC::GetDistrict(const FString& DistrictName)
{
    FNYCDistrict* DistrictPtr = Districts.Find(DistrictName);
    return DistrictPtr ? *DistrictPtr : FNYCDistrict();
}

TArray<FBuildingData> UProceduralNYC::GetBuildingsInRadius(const FVector& Location, float Radius)
{
    TArray<FBuildingData> ResultBuildings;
    
    for (auto& DistrictPair : Districts)
    {
        for (const FBuildingData& Building : DistrictPair.Value.Buildings)
        {
            if (FVector::Dist(Building.BaseLocation, Location) < Radius)
            {
                ResultBuildings.Add(Building);
            }
        }
    }
    
    return ResultBuildings;
}

FStreetSegment UProceduralNYC::GetNearestStreet(const FVector& Location)
{
    float NearestDistance = FLT_MAX;
    FStreetSegment NearestStreet;
    
    for (auto& DistrictPair : Districts)
    {
        for (const FStreetSegment& Street : DistrictPair.Value.Streets)
        {
            FVector ClosestPoint = FMath::ClosestPointOnLineSegment(Location, Street.StartPoint, Street.EndPoint);
            float Distance = FVector::Dist(Location, ClosestPoint);
            
            if (Distance < NearestDistance)
            {
                NearestDistance = Distance;
                NearestStreet = Street;
            }
        }
    }
    
    return NearestStreet;
}

void UProceduralNYC::CreateFractureZoneGeometry(const FVector& CenterLocation, float Radius)
{
    // Create impossible architecture in fracture zones
    FNYCDistrict FractureDistrict;
    FractureDistrict.DistrictName = TEXT("FractureZone_") + FString::FromInt(FMath::Rand());
    FractureDistrict.DistrictCenter = CenterLocation;
    FractureDistrict.DistrictRadius = Radius;
    FractureDistrict.ZoneType = ERZ_Fracture;
    FractureDistrict.CorruptionLevel = 1.0f;
    FractureDistrict.bIsGenerated = true;
    
    Districts.Add(FractureDistrict.DistrictName, FractureDistrict);
}

void UProceduralNYC::CreateNullZoneGeometry(const FVector& CenterLocation, float Radius)
{
    // Create liminal backroom-style architecture
    FNYCDistrict NullDistrict;
    NullDistrict.DistrictName = TEXT("NullZone_") + FString::FromInt(FMath::Rand());
    NullDistrict.DistrictCenter = CenterLocation;
    NullDistrict.DistrictRadius = Radius;
    NullDistrict.ZoneType = ERZ_Null;
    NullDistrict.CorruptionLevel = 0.5f;
    NullDistrict.bIsGenerated = true;
    
    // Generate infinite corridor-like streets
    for (int32 i = 0; i < 10; ++i)
    {
        FStreetSegment EndlessHallway;
        EndlessHallway.StartPoint = CenterLocation + FVector(0, i * 200.0f, 0);
        EndlessHallway.EndPoint = EndlessHallway.StartPoint + FVector(10000.0f, 0, 0);  // Very long street
        EndlessHallway.Width = 50.0f;
        NullDistrict.Streets.Add(EndlessHallway);
    }
    
    Districts.Add(NullDistrict.DistrictName, NullDistrict);
}

void UProceduralNYC::StreamInRegion(const FVector& RegionCenter, float StreamRadius)
{
    // Generate geometry in this region if it doesn't exist
    bool bFoundDistrict = false;
    
    for (auto& DistrictPair : Districts)
    {
        if (FVector::Dist(DistrictPair.Value.DistrictCenter, RegionCenter) < StreamRadius)
        {
            bFoundDistrict = true;
            break;
        }
    }
    
    if (!bFoundDistrict)
    {
        GenerateDistrict(TEXT("StreamedDistrict_") + FString::FromInt(FMath::Rand()), RegionCenter, StreamRadius);
    }
}

void UProceduralNYC::StreamOutRegion(const FVector& RegionCenter)
{
    // Unload geometry outside active regions
    for (auto It = Districts.CreateIterator(); It; ++It)
    {
        if (It.Value().bIsGenerated && FVector::Dist(It.Value().DistrictCenter, RegionCenter) > 10000.0f)
        {
            It.RemoveCurrent();
        }
    }
}

void UProceduralNYC::UpdateMutations(float DeltaTime)
{
    // Randomly mutate districts that aren't being observed
    for (auto& DistrictPair : Districts)
    {
        if (FMath::FRand() < MutationRate * DeltaTime)
        {
            MutateDistrictLayout(DistrictPair.Key);
        }
    }
}

void UProceduralNYC::ProcessLockedGeometry()
{
    // Ensure locked geometry remains stable
}

FVector UProceduralNYC::GenerateRandomBuildingLocation(const FVector& CenterLocation, float Radius)
{
    float Angle = FMath::FRand() * 2.0f * PI;
    float Distance = FMath::FRand() * Radius;
    
    FVector BuildingLocation;
    BuildingLocation.X = CenterLocation.X + FMath::Cos(Angle) * Distance;
    BuildingLocation.Y = CenterLocation.Y + FMath::Sin(Angle) * Distance;
    BuildingLocation.Z = CenterLocation.Z;
    
    return BuildingLocation;
}

FBuildingData UProceduralNYC::CreateBuilding(const FVector& Location, int32 BuildingType)
{
    FBuildingData Building;
    Building.BaseLocation = Location;
    Building.CurrentLocation = Location;
    Building.Scale = FVector(FMath::FRandRange(1.0f, 3.0f), FMath::FRandRange(1.0f, 3.0f), FMath::FRandRange(2.0f, 5.0f));
    Building.Rotation = FRotator(0, FMath::FRand() * 360.0f, 0);
    Building.BuildingType = BuildingType;
    Building.Floors = FMath::RandRange(5, 30);
    Building.bIsGenerated = true;
    Building.bIsObserved = false;
    Building.LocationHash = GenerateDeterministicHash(Location);
    
    return Building;
}

uint32 UProceduralNYC::GenerateDeterministicHash(const FVector& Location)
{
    uint32 Hash = *(uint32*)&Location.X;
    Hash ^= *(uint32*)&Location.Y;
    Hash ^= *(uint32*)&Location.Z;
    return Hash;
}
