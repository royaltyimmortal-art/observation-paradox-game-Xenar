#include "Core/ObservableActor.h"
#include "Core/QuantumObservationSystem.h"

AObservableActor::AObservableActor()
    : ObservationDecayTime(2.0f)
    , CurrentObservationDuration(0.0f)
    , TimeSinceLastObserved(0.0f)
    , CurrentGlitchIntensity(0.0f)
    , GlitchDuration(0.0f)
    , bCanBeUnobserved(true)
    , bIsFracturable(true)
    , UnobservedMovementRadius(500.0f)
    , StateTransitionSpeed(1.0f)
{
    PrimaryActorTick.TickInterval = 0.0f;
    PrimaryActorTick.TickType = ETickableTickType::Always;
}

void AObservableActor::BeginPlay()
{
    Super::BeginPlay();
    
    // Register with the quantum observation system
    RegisterWithSystem();
    
    // Store initial state
    StoredLocation = GetActorLocation();
    StoredRotation = GetActorRotation();
    StoredScale = GetActorScale3D();
    
    // Initialize current state
    CurrentState.Location = StoredLocation;
    CurrentState.Rotation = StoredRotation;
    CurrentState.Scale = StoredScale;
    CurrentState.bVisible = true;
    CurrentState.bPhysicsEnabled = true;
}

void AObservableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnregisterFromSystem();
    Super::EndPlay(EndPlayReason);
}

void AObservableActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    UpdateQuantumState(DeltaTime);
    ApplyGlitchVisuals(DeltaTime);
    HandleObservationDecay(DeltaTime);
}

void AObservableActor::OnObserved()
{
    CurrentObservationDuration = 0.0f;
    TimeSinceLastObserved = 0.0f;
    OnBeingWatched();
}

void AObservableActor::OnUnobserved()
{
    CurrentObservationDuration = 0.0f;
    OnNoLongerWatched();
}

void AObservableActor::OnObservationLost()
{
    // Called when observation period ends
}

void AObservableActor::RegisterQuantumStates(const TArray<FQuantumState>& InStates)
{
    PossibleStates = InStates;
}

FQuantumState AObservableActor::GetCurrentQuantumState() const
{
    return CurrentState;
}

void AObservableActor::ApplyQuantumState(const FQuantumState& NewState)
{
    // Smoothly transition to new state
    CurrentState = NewState;
    
    if (NewState.bVisible)
    {
        SetActorHiddenInGame(false);
    }
    else
    {
        SetActorHiddenInGame(true);
    }
    
    // Lerp to new location for smooth transition
    FVector CurrentLoc = GetActorLocation();
    FVector NewLoc = FMath::Lerp(CurrentLoc, NewState.Location, StateTransitionSpeed * 0.016f);
    SetActorLocation(NewLoc);
}

void AObservableActor::SetRealityZone(ERealityZone NewZone)
{
    CurrentRealityZone = NewZone;
}

EObservationState AObservableActor::GetObservationState() const
{
    return FQuantumObservationSystem::Get().GetObservationState(this);
}

void AObservableActor::TriggerGlitch(float Intensity, float Duration)
{
    CurrentGlitchIntensity = Intensity;
    GlitchDuration = Duration;
}

void AObservableActor::ShiftToRandomState()
{
    if (PossibleStates.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, PossibleStates.Num() - 1);
        ApplyQuantumState(PossibleStates[RandomIndex]);
    }
}

void AObservableActor::EnterFractureState()
{
    CurrentGlitchIntensity = 1.0f;
    ObservationState = EOS_Fractured;
}

void AObservableActor::UpdateQuantumState(float DeltaTime)
{
    EObservationState State = GetObservationState();
    
    if (State == EOS_Unobserved && bCanBeUnobserved)
    {
        // When unobserved, actor can move to random position
        if (FMath::FRand() > 0.98f)
        {
            FVector RandomOffset;
            RandomOffset.X = FMath::FRandRange(-UnobservedMovementRadius, UnobservedMovementRadius);
            RandomOffset.Y = FMath::FRandRange(-UnobservedMovementRadius, UnobservedMovementRadius);
            RandomOffset.Z = FMath::FRandRange(-100.0f, 100.0f);
            
            ShiftToRandomState();
        }
    }
}

void AObservableActor::ApplyGlitchVisuals(float DeltaTime)
{
    if (CurrentGlitchIntensity > 0.0f)
    {
        GlitchDuration -= DeltaTime;
        
        if (GlitchDuration <= 0.0f)
        {
            CurrentGlitchIntensity = 0.0f;
        }
        else
        {
            // Apply glitch effect
            FVector GlitchOffset;
            GlitchOffset.X = FMath::Sin(GetWorld()->GetTimeSeconds() * 20.0f) * CurrentGlitchIntensity * 10.0f;
            GlitchOffset.Y = FMath::Cos(GetWorld()->GetTimeSeconds() * 20.0f) * CurrentGlitchIntensity * 10.0f;
            GlitchOffset.Z = FMath::Sin(GetWorld()->GetTimeSeconds() * 15.0f) * CurrentGlitchIntensity * 5.0f;
            
            SetActorLocation(StoredLocation + GlitchOffset);
        }
    }
}

void AObservableActor::HandleObservationDecay(float DeltaTime)
{
    EObservationState State = GetObservationState();
    
    if (State == EOS_Observed)
    {
        CurrentObservationDuration += DeltaTime;
    }
    else if (State == EOS_Unobserved)
    {
        TimeSinceLastObserved += DeltaTime;
        
        if (TimeSinceLastObserved > ObservationDecayTime && bCanBeUnobserved)
        {
            // Trigger uncertainty flux
            FQuantumObservationSystem::Get().UncertaintyFlux(this);
        }
    }
}

void AObservableActor::RegisterWithSystem()
{
    FQuantumObservationSystem::Get().RegisterObservable(this);
}

void AObservableActor::UnregisterFromSystem()
{
    FQuantumObservationSystem::Get().UnregisterObservable(this);
}
