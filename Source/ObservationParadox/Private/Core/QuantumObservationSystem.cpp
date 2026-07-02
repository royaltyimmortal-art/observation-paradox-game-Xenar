#include "Core/QuantumObservationSystem.h"
#include "Core/ObservableActor.h"
#include "Containers/List.h"
#include "Math/RandomStream.h"

/**
 * QUANTUM OBSERVATION SYSTEM IMPLEMENTATION
 * Core mechanics for observation-based reality manipulation
 */

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
    // Initialize reality zone map
    RealityZoneMap.Empty();
    FractureZones.Empty();
    LocalTimeOffsets.Empty();
    
    UE_LOG(LogTemp, Warning, TEXT("Quantum Observation System Initialized"));
}

void FQuantumObservationSystem::Shutdown()
{
    RegisteredObservables.Empty();
    ObservationStates.Empty();
    CurrentObservations.Empty();
    PossibleStates.Empty();
    LocalTimeOffsets.Empty();
    
    UE_LOG(LogTemp, Warning, TEXT("Quantum Observation System Shutdown"));
}

void FQuantumObservationSystem::Update(float DeltaTime)
{
    LastUpdateTime = DeltaTime;
    
    // Update all observation states
    UpdateObservationStates(DeltaTime);
    
    // Process uncertainty decay
    ProcessUncertaintyDecay(DeltaTime);
    
    // Apply quantum effects to observable actors
    ApplyQuantumEffects(DeltaTime);
}

void FQuantumObservationSystem::RegisterObservable(AObservableActor* InActor)
{
    if (!InActor)
    {
        return;
    }
    
    if (RegisteredObservables.Num() >= MaxObservableActors)
    {
        UE_LOG(LogTemp, Warning, TEXT("Max observable actors reached!"));
        return;
    }
    
    RegisteredObservables.Add(InActor);
    ObservationStates.Add(InActor, EOS_Unobserved);
    PossibleStates.Add(InActor, TArray<FQuantumState>());
    
    // Generate initial quantum states
    TArray<FQuantumState> InitialStates = GenerateMultipleStates(InActor, 5);
    PossibleStates[InActor] = InitialStates;
}

void FQuantumObservationSystem::UnregisterObservable(AObservableActor* InActor)
{
    if (!InActor)
    {
        return;
    }
    
    RegisteredObservables.Remove(InActor);
    ObservationStates.Remove(InActor);
    CurrentObservations.Remove(InActor);
    PossibleStates.Remove(InActor);
    LocalTimeOffsets.Remove(InActor);
}

void FQuantumObservationSystem::SetPlayerObservation(const FObservationData& ObservationData)
{
    if (!ObservationData.ObservedActor)
    {
        return;
    }
    
    CurrentObservations.Add(ObservationData.ObservedActor, ObservationData);
    
    // Collapse reality for this actor
    CollapseReality(ObservationData.ObservedActor);
}

void FQuantumObservationSystem::ClearPlayerObservation()
{
    CurrentObservations.Empty();
}

void FQuantumObservationSystem::CollapseReality(AObservableActor* InActor)
{
    if (!InActor)
    {
        return;
    }
    
    // Lock the actor's state - no more uncertainty
    ObservationStates.Add(InActor, EOS_Observed);
    
    // Notify the actor it's being observed
    InActor->OnObserved();
}

void FQuantumObservationSystem::UncertaintyFlux(AObservableActor* InActor)
{
    if (!InActor)
    {
        return;
    }
    
    // Put the actor into quantum flux - multiple states possible
    ObservationStates.Add(InActor, EOS_Uncertainty);
    
    // Generate new possible states
    TArray<FQuantumState> NewStates = GenerateMultipleStates(InActor, 8);
    PossibleStates[InActor] = NewStates;
    
    // Randomly select and apply one
    if (NewStates.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, NewStates.Num() - 1);
        InActor->ApplyQuantumState(NewStates[RandomIndex]);
    }
}

void FQuantumObservationSystem::TriggerFracture(AObservableActor* InActor)
{
    if (!InActor)
    {
        return;
    }
    
    ObservationStates.Add(InActor, EOS_Fractured);
    InActor->EnterFractureState();
}

EObservationState FQuantumObservationSystem::GetObservationState(AObservableActor* InActor) const
{
    if (!InActor)
    {
        return EOS_Unobserved;
    }
    
    const EObservationState* StatePtr = ObservationStates.Find(InActor);
    return StatePtr ? *StatePtr : EOS_Unobserved;
}

ERealityZone FQuantumObservationSystem::GetRealityZone(const FVector& Location) const
{
    // Check which zone this location is in
    for (const auto& ZonePair : RealityZoneMap)
    {
        // Simple distance check - can be optimized with spatial hashing
        if (FVector::Dist(ZonePair.Key, Location) < 1000.0f)
        {
            return ZonePair.Value;
        }
    }
    
    return ERZ_Stable;
}

FQuantumState FQuantumObservationSystem::GenerateUncertaintyState(AObservableActor* InActor)
{
    if (!InActor)
    {
        return FQuantumState();
    }
    
    FQuantumState NewState;
    NewState.Location = InActor->GetActorLocation();
    NewState.Location += CalculateUncertaintyVariation(NewState.Location);
    NewState.Rotation = InActor->GetActorRotation();
    NewState.Scale = InActor->GetActorScale3D();
    NewState.bVisible = FMath::RandBool();
    NewState.bPhysicsEnabled = FMath::RandBool();
    NewState.VariationID = FMath::Rand() % 255;
    NewState.Probability = FMath::FRand();
    NewState.StateHash = CalculateStateHash(NewState);
    
    return NewState;
}

TArray<FQuantumState> FQuantumObservationSystem::GenerateMultipleStates(AObservableActor* InActor, int32 Count)
{
    TArray<FQuantumState> States;
    
    for (int32 i = 0; i < Count; ++i)
    {
        States.Add(GenerateUncertaintyState(InActor));
    }
    
    return States;
}

void FQuantumObservationSystem::ApplyGlitchEffect(AObservableActor* InActor, float Intensity)
{
    if (!InActor)
    {
        return;
    }
    
    // Glitch effect will distort the actor
    FQuantumState CurrentState = InActor->GetCurrentQuantumState();
    CurrentState.Location += CalculateUncertaintyVariation(CurrentState.Location) * Intensity;
    InActor->ApplyQuantumState(CurrentState);
}

void FQuantumObservationSystem::ShiftGeometry(AObservableActor* InActor, const FVector& TargetLocation)
{
    if (!InActor)
    {
        return;
    }
    
    InActor->SetActorLocation(TargetLocation);
}

void FQuantumObservationSystem::CreateFractureZone(const FVector& CenterLocation, float Radius)
{
    FractureZones.Add(CenterLocation);
    RealityZoneMap.Add(CenterLocation, ERZ_Fracture);
}

void FQuantumObservationSystem::DesynchronizeTime(AObservableActor* InActor, float TimeOffset)
{
    if (!InActor)
    {
        return;
    }
    
    LocalTimeOffsets.Add(InActor, TimeOffset);
}

float FQuantumObservationSystem::GetLocalTimeMultiplier(const FVector& Location) const
{
    // Check if location is in a reality zone with time distortion
    ERealityZone Zone = GetRealityZone(Location);
    
    switch (Zone)
    {
        case ERZ_Stable:
            return 1.0f;
        case ERZ_Fracture:
            return FMath::RandRange(0.5f, 1.5f);  // Time behaves erratically
        case ERZ_Null:
            return 0.1f;  // Time nearly stops
        default:
            return 1.0f;
    }
}

FQuantumObservationSystem& FQuantumObservationSystem::Get()
{
    static FQuantumObservationSystem Instance;
    return Instance;
}

void FQuantumObservationSystem::UpdateObservationStates(float DeltaTime)
{
    for (auto It = ObservationStates.CreateIterator(); It; ++It)
    {
        AObservableActor* Actor = It.Key().Get();
        EObservationState& State = It.Value();
        
        if (!Actor)
        {
            It.RemoveCurrent();
            continue;
        }
        
        // Check if actor is still being observed
        if (!CurrentObservations.Contains(Actor))
        {
            // Actor is no longer observed - transition to uncertainty
            if (State == EOS_Observed)
            {
                State = EOS_Uncertainty;
                Actor->OnUnobserved();
            }
        }
    }
}

void FQuantumObservationSystem::ProcessUncertaintyDecay(float DeltaTime)
{
    for (auto& ObservationPair : CurrentObservations)
    {
        AObservableActor* Actor = ObservationPair.Key.Get();
        FObservationData& ObsData = ObservationPair.Value;
        
        if (Actor)
        {
            ObsData.ObservationDuration += DeltaTime;
        }
    }
}

void FQuantumObservationSystem::ApplyQuantumEffects(float DeltaTime)
{
    for (auto& StatePair : ObservationStates)
    {
        AObservableActor* Actor = StatePair.Key.Get();
        EObservationState State = StatePair.Value;
        
        if (!Actor)
        {
            continue;
        }
        
        // Apply state-specific effects
        switch (State)
        {
            case EOS_Unobserved:
                // Actor can change freely
                UncertaintyFlux(Actor);
                break;
                
            case EOS_Uncertainty:
                // Actor flickers between states
                if (FMath::FRand() > 0.95f)
                {
                    UncertaintyFlux(Actor);
                }
                break;
                
            case EOS_Fractured:
                // Actor exhibits extreme distortion
                ApplyGlitchEffect(Actor, 0.5f);
                break;
                
            default:
                break;
        }
    }
}

uint8 FQuantumObservationSystem::CalculateStateHash(const FQuantumState& State) const
{
    uint8 Hash = 0;
    Hash ^= (uint8)(State.Location.X * 37);
    Hash ^= (uint8)(State.Location.Y * 47);
    Hash ^= (uint8)(State.Location.Z * 53);
    Hash ^= State.VariationID;
    return Hash;
}

FVector FQuantumObservationSystem::CalculateUncertaintyVariation(const FVector& BaseLocation)
{
    // Generate pseudo-random variation based on location
    FRandomStream RandomStream(*(uint32*)&BaseLocation.X);
    FVector Variation;
    Variation.X = RandomStream.FRandRange(-500.0f, 500.0f);
    Variation.Y = RandomStream.FRandRange(-500.0f, 500.0f);
    Variation.Z = RandomStream.FRandRange(-250.0f, 250.0f);
    return Variation;
}
