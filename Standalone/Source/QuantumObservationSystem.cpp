#include "QuantumObservationSystem.h"
#include "ObservableActor.h"
#include <algorithm>
#include <iostream>

namespace ObservationParadox {

static FQuantumObservationSystem* GQuantumSystem = nullptr;

FQuantumObservationSystem::FQuantumObservationSystem()
    : GlobalTimeScale(1.0f)
    , LastUpdateTime(0.0f)
    , MaxObservableActors(10000)
    , ObservationDecayTime(2.0f)
{
}

FQuantumObservationSystem::~FQuantumObservationSystem()
{
    Shutdown();
}

void FQuantumObservationSystem::Initialize()
{
    RealityZoneMap.clear();
    FractureZones.clear();
    LocalTimeOffsets.clear();
    std::cout << "[QUANTUM SYSTEM] Initialized" << std::endl;
}

void FQuantumObservationSystem::Shutdown()
{
    RegisteredObservables.clear();
    ObservationStates.clear();
    CurrentObservations.clear();
    PossibleStates.clear();
    LocalTimeOffsets.clear();
    std::cout << "[QUANTUM SYSTEM] Shutdown" << std::endl;
}

void FQuantumObservationSystem::Update(float DeltaTime)
{
    LastUpdateTime = DeltaTime;
    UpdateObservationStates(DeltaTime);
    ProcessUncertaintyDecay(DeltaTime);
    ApplyQuantumEffects(DeltaTime);
}

void FQuantumObservationSystem::RegisterObservable(AObservableActor* InActor)
{
    if (!InActor || RegisteredObservables.size() >= MaxObservableActors)
        return;

    RegisteredObservables.push_back(InActor);
    ObservationStates[InActor] = EObservationState::Unobserved;
    PossibleStates[InActor] = std::vector<FQuantumState>();

    std::vector<FQuantumState> InitialStates = GenerateMultipleStates(InActor, 5);
    PossibleStates[InActor] = InitialStates;
}

void FQuantumObservationSystem::UnregisterObservable(AObservableActor* InActor)
{
    if (!InActor) return;

    auto it = std::find(RegisteredObservables.begin(), RegisteredObservables.end(), InActor);
    if (it != RegisteredObservables.end())
        RegisteredObservables.erase(it);

    ObservationStates.erase(InActor);
    CurrentObservations.erase(InActor);
    PossibleStates.erase(InActor);
    LocalTimeOffsets.erase(InActor);
}

void FQuantumObservationSystem::SetPlayerObservation(const FObservationData& ObservationData)
{
    if (!ObservationData.ObservedActor) return;

    AObservableActor* Actor = static_cast<AObservableActor*>(ObservationData.ObservedActor);
    CurrentObservations[Actor] = ObservationData;
    CollapseReality(Actor);
}

void FQuantumObservationSystem::ClearPlayerObservation()
{
    CurrentObservations.clear();
}

void FQuantumObservationSystem::CollapseReality(AObservableActor* InActor)
{
    if (!InActor) return;

    ObservationStates[InActor] = EObservationState::Observed;
    InActor->OnObserved();
}

void FQuantumObservationSystem::UncertaintyFlux(AObservableActor* InActor)
{
    if (!InActor) return;

    ObservationStates[InActor] = EObservationState::Uncertainty;
    std::vector<FQuantumState> NewStates = GenerateMultipleStates(InActor, 8);
    PossibleStates[InActor] = NewStates;

    if (!NewStates.empty())
    {
        int RandomIndex = Math::RandRange(0, NewStates.size() - 1);
        InActor->ApplyQuantumState(NewStates[RandomIndex]);
    }
}

void FQuantumObservationSystem::TriggerFracture(AObservableActor* InActor)
{
    if (!InActor) return;

    ObservationStates[InActor] = EObservationState::Fractured;
    InActor->EnterFractureState();
}

EObservationState FQuantumObservationSystem::GetObservationState(AObservableActor* InActor) const
{
    if (!InActor) return EObservationState::Unobserved;

    auto it = ObservationStates.find(InActor);
    return it != ObservationStates.end() ? it->second : EObservationState::Unobserved;
}

ERealityZone FQuantumObservationSystem::GetRealityZone(const FVector& Location) const
{
    for (const auto& ZonePair : RealityZoneMap)
    {
        if (FVector::Distance(ZonePair.first, Location) < 1000.0f)
            return ZonePair.second;
    }
    return ERealityZone::Stable;
}

FQuantumState FQuantumObservationSystem::GenerateUncertaintyState(AObservableActor* InActor)
{
    if (!InActor) return FQuantumState();

    FQuantumState NewState;
    NewState.Location = InActor->GetActorLocation();
    NewState.Location = NewState.Location + CalculateUncertaintyVariation(NewState.Location);
    NewState.Rotation = InActor->GetActorRotation();
    NewState.Scale = InActor->GetActorScale();
    NewState.bVisible = Math::RandBool();
    NewState.bPhysicsEnabled = Math::RandBool();
    NewState.VariationID = Math::RandRange(0, 255);
    NewState.Probability = Math::FRand();
    NewState.StateHash = CalculateStateHash(NewState);

    return NewState;
}

std::vector<FQuantumState> FQuantumObservationSystem::GenerateMultipleStates(AObservableActor* InActor, int Count)
{
    std::vector<FQuantumState> States;
    for (int i = 0; i < Count; ++i)
    {
        States.push_back(GenerateUncertaintyState(InActor));
    }
    return States;
}

void FQuantumObservationSystem::ApplyGlitchEffect(AObservableActor* InActor, float Intensity)
{
    if (!InActor) return;

    FQuantumState CurrentState = InActor->GetCurrentQuantumState();
    CurrentState.Location = CurrentState.Location + (CalculateUncertaintyVariation(CurrentState.Location) * Intensity);
    InActor->ApplyQuantumState(CurrentState);
}

void FQuantumObservationSystem::ShiftGeometry(AObservableActor* InActor, const FVector& TargetLocation)
{
    if (!InActor) return;
    InActor->SetActorLocation(TargetLocation);
}

void FQuantumObservationSystem::CreateFractureZone(const FVector& CenterLocation, float Radius)
{
    FractureZones.push_back(CenterLocation);
    RealityZoneMap[CenterLocation] = ERealityZone::Fracture;
}

void FQuantumObservationSystem::DesynchronizeTime(AObservableActor* InActor, float TimeOffset)
{
    if (!InActor) return;
    LocalTimeOffsets[InActor] = TimeOffset;
}

float FQuantumObservationSystem::GetLocalTimeMultiplier(const FVector& Location) const
{
    ERealityZone Zone = GetRealityZone(Location);
    switch (Zone)
    {
        case ERealityZone::Stable:
            return 1.0f;
        case ERealityZone::Fracture:
            return Math::FRandRange(0.5f, 1.5f);
        case ERealityZone::Null:
            return 0.1f;
        default:
            return 1.0f;
    }
}

FQuantumObservationSystem& FQuantumObservationSystem::Get()
{
    if (!GQuantumSystem)
        GQuantumSystem = new FQuantumObservationSystem();
    return *GQuantumSystem;
}

void FQuantumObservationSystem::UpdateObservationStates(float DeltaTime)
{
    for (auto it = ObservationStates.begin(); it != ObservationStates.end(); ++it)
    {
        AObservableActor* Actor = it->first;
        if (!Actor) continue;

        if (CurrentObservations.find(Actor) == CurrentObservations.end())
        {
            if (it->second == EObservationState::Observed)
            {
                it->second = EObservationState::Uncertainty;
                Actor->OnUnobserved();
            }
        }
    }
}

void FQuantumObservationSystem::ProcessUncertaintyDecay(float DeltaTime)
{
    for (auto& ObsPair : CurrentObservations)
    {
        AObservableActor* Actor = ObsPair.first;
        FObservationData& ObsData = ObsPair.second;
        if (Actor)
            ObsData.ObservationDuration += DeltaTime;
    }
}

void FQuantumObservationSystem::ApplyQuantumEffects(float DeltaTime)
{
    for (auto& StatePair : ObservationStates)
    {
        AObservableActor* Actor = StatePair.first;
        EObservationState State = StatePair.second;
        if (!Actor) continue;

        switch (State)
        {
            case EObservationState::Unobserved:
                UncertaintyFlux(Actor);
                break;
            case EObservationState::Uncertainty:
                if (Math::FRand() > 0.95f)
                    UncertaintyFlux(Actor);
                break;
            case EObservationState::Fractured:
                ApplyGlitchEffect(Actor, 0.5f);
                break;
            default:
                break;
        }
    }
}

uint8_t FQuantumObservationSystem::CalculateStateHash(const FQuantumState& State) const
{
    uint8_t Hash = 0;
    Hash ^= (uint8_t)(State.Location.X * 37);
    Hash ^= (uint8_t)(State.Location.Y * 47);
    Hash ^= (uint8_t)(State.Location.Z * 53);
    Hash ^= State.VariationID;
    return Hash;
}

FVector FQuantumObservationSystem::CalculateUncertaintyVariation(const FVector& BaseLocation)
{
    FVector Variation;
    Variation.X = Math::FRandRange(-500.0f, 500.0f);
    Variation.Y = Math::FRandRange(-500.0f, 500.0f);
    Variation.Z = Math::FRandRange(-250.0f, 250.0f);
    return Variation;
}

}  // namespace ObservationParadox
