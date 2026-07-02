#include "RealityManager.h"
#include <iostream>

namespace ObservationParadox {

static URealityManager* GRealityManager = nullptr;

URealityManager::URealityManager()
    : GlobalCorruptionLevel(0.0f)
    , GlobalTimeScale(1.0f)
    , CorruptionSpreadRate(0.1f)
    , MaxCorruptionLevel(1.0f)
    , ZoneTransitionSpeed(1.0f)
{
}

URealityManager::~URealityManager()
{
    Deinitialize();
}

void URealityManager::Initialize()
{
    std::cout << "[REALITY MANAGER] Initialized" << std::endl;
}

void URealityManager::Deinitialize()
{
    ActiveZones.clear();
}

void URealityManager::Update(float DeltaTime)
{
    UpdateAllZones(DeltaTime);
    CheckAndApplyZoneTransitions();
    UpdateCorruptionEffects(DeltaTime);
    ProcessEnvironmentalChanges(DeltaTime);
}

void URealityManager::CreateRealityZone(const FVector& Center, float Radius, ERealityZone ZoneType)
{
    FRealityZoneData NewZone;
    NewZone.ZoneCenter = Center;
    NewZone.ZoneRadius = Radius;
    NewZone.ZoneType = ZoneType;
    NewZone.TimeMultiplier = 1.0f;
    NewZone.CorruptionLevel = 0.0f;
    NewZone.bIsActive = true;
    ActiveZones.push_back(NewZone);
}

void URealityManager::RemoveRealityZone(FVector ZoneCenter)
{
    for (int i = ActiveZones.size() - 1; i >= 0; --i)
    {
        if (FVector::Distance(ActiveZones[i].ZoneCenter, ZoneCenter) < 100.0f)
        {
            ActiveZones.erase(ActiveZones.begin() + i);
        }
    }
}

void URealityManager::UpdateZoneCorruption(FVector ZoneCenter, float CorruptionDelta)
{
    for (FRealityZoneData& Zone : ActiveZones)
    {
        if (FVector::Distance(Zone.ZoneCenter, ZoneCenter) < Zone.ZoneRadius)
        {
            Zone.CorruptionLevel = Math::Clamp(Zone.CorruptionLevel + CorruptionDelta, 0.0f, 1.0f);
        }
    }
}

FRealityZoneData URealityManager::GetZoneAtLocation(const FVector& Location)
{
    for (const FRealityZoneData& Zone : ActiveZones)
    {
        if (FVector::Distance(Zone.ZoneCenter, Location) < Zone.ZoneRadius)
        {
            return Zone;
        }
    }

    FRealityZoneData StableZone;
    StableZone.ZoneType = ERealityZone::Stable;
    StableZone.TimeMultiplier = 1.0f;
    StableZone.CorruptionLevel = 0.0f;
    return StableZone;
}

void URealityManager::TransitionToZone(AObservableActor* InActor, ERealityZone TargetZone)
{
    if (!InActor) return;
}

void URealityManager::ApplyZoneEffects(const FVector& Location, float DeltaTime)
{
    FRealityZoneData Zone = GetZoneAtLocation(Location);

    switch (Zone.ZoneType)
    {
        case ERealityZone::Stable:
            break;
        case ERealityZone::Fracture:
            GlobalTimeScale = Math::Lerp(GlobalTimeScale, Zone.TimeMultiplier, DeltaTime * 0.1f);
            break;
        case ERealityZone::Null:
            GlobalTimeScale = Math::Lerp(GlobalTimeScale, 0.1f, DeltaTime * 0.1f);
            break;
        default:
            break;
    }
}

void URealityManager::SpreadCorruption(const FVector& CorruptionPoint, float CorruptionAmount)
{
    GlobalCorruptionLevel = Math::Clamp(GlobalCorruptionLevel + CorruptionAmount, 0.0f, MaxCorruptionLevel);

    for (FRealityZoneData& Zone : ActiveZones)
    {
        float Distance = FVector::Distance(Zone.ZoneCenter, CorruptionPoint);
        if (Distance < Zone.ZoneRadius * 2.0f)
        {
            Zone.CorruptionLevel += CorruptionAmount * (1.0f - Distance / (Zone.ZoneRadius * 2.0f));
            Zone.CorruptionLevel = Math::Clamp(Zone.CorruptionLevel, 0.0f, 1.0f);
        }
    }
}

ERealityZone URealityManager::GetRealityZoneAtLocation(const FVector& Location)
{
    return GetZoneAtLocation(Location).ZoneType;
}

float URealityManager::GetTimeMultiplierAtLocation(const FVector& Location)
{
    return GetZoneAtLocation(Location).TimeMultiplier;
}

float URealityManager::GetCorruptionLevelAtLocation(const FVector& Location)
{
    return GetZoneAtLocation(Location).CorruptionLevel;
}

void URealityManager::TriggerGlobalFracture(float Intensity)
{
    for (FRealityZoneData& Zone : ActiveZones)
    {
        Zone.CorruptionLevel = Math::Clamp(1.0f, Zone.CorruptionLevel + Intensity);
        Zone.TimeMultiplier = Math::Lerp(Zone.TimeMultiplier, Math::FRandRange(0.5f, 2.0f), 0.5f);
    }
    GlobalCorruptionLevel = Math::Clamp(GlobalCorruptionLevel + Intensity, 0.0f, MaxCorruptionLevel);
}

void URealityManager::InitiateParadoxLoop()
{
}

void URealityManager::RestabilizeReality()
{
    GlobalCorruptionLevel = Math::Clamp(GlobalCorruptionLevel - 0.05f, 0.0f, 1.0f);

    for (FRealityZoneData& Zone : ActiveZones)
    {
        Zone.CorruptionLevel = Math::Clamp(Zone.CorruptionLevel - 0.05f, 0.0f, 1.0f);
        Zone.TimeMultiplier = Math::Lerp(Zone.TimeMultiplier, 1.0f, 0.1f);
    }
}

URealityManager& URealityManager::Get()
{
    if (!GRealityManager)
        GRealityManager = new URealityManager();
    return *GRealityManager;
}

void URealityManager::UpdateAllZones(float DeltaTime)
{
    for (FRealityZoneData& Zone : ActiveZones)
    {
        Zone.LastUpdateTime += DeltaTime;
    }
}

void URealityManager::CheckAndApplyZoneTransitions()
{
}

void URealityManager::UpdateCorruptionEffects(float DeltaTime)
{
    if (GlobalCorruptionLevel > 0.0f)
    {
        for (FRealityZoneData& Zone : ActiveZones)
        {
            Zone.CorruptionLevel += CorruptionSpreadRate * DeltaTime * GlobalCorruptionLevel;
            Zone.CorruptionLevel = Math::Clamp(Zone.CorruptionLevel, 0.0f, 1.0f);
        }
    }
}

void URealityManager::ProcessEnvironmentalChanges(float DeltaTime)
{
}

}  // namespace ObservationParadox
