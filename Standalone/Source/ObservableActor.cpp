#include "ObservableActor.h"
#include "QuantumObservationSystem.h"
#include <iostream>

namespace ObservationParadox {

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
    , bHidden(false)
{
}

AObservableActor::~AObservableActor()
{
}

void AObservableActor::BeginPlay()
{
    RegisterWithSystem();
    StoredLocation = Location;
    StoredRotation = Rotation;
    StoredScale = Scale;

    CurrentState.Location = StoredLocation;
    CurrentState.Rotation = StoredRotation;
    CurrentState.Scale = StoredScale;
    CurrentState.bVisible = true;
    CurrentState.bPhysicsEnabled = true;
}

void AObservableActor::EndPlay()
{
    UnregisterFromSystem();
}

void AObservableActor::Tick(float DeltaTime)
{
    UpdateQuantumState(DeltaTime);
    ApplyGlitchVisuals(DeltaTime);
    HandleObservationDecay(DeltaTime);
}

void AObservableActor::OnObserved()
{
    CurrentObservationDuration = 0.0f;
    TimeSinceLastObserved = 0.0f;
}

void AObservableActor::OnUnobserved()
{
    CurrentObservationDuration = 0.0f;
}

void AObservableActor::OnObservationLost()
{
}

void AObservableActor::RegisterQuantumStates(const std::vector<FQuantumState>& InStates)
{
    PossibleStates = InStates;
}

void AObservableActor::ApplyQuantumState(const FQuantumState& NewState)
{
    CurrentState = NewState;
    bHidden = !NewState.bVisible;
    Location = NewState.Location;
}

EObservationState AObservableActor::GetObservationState() const
{
    return FQuantumObservationSystem::Get().GetObservationState(const_cast<AObservableActor*>(this));
}

void AObservableActor::TriggerGlitch(float Intensity, float Duration)
{
    CurrentGlitchIntensity = Intensity;
    GlitchDuration = Duration;
}

void AObservableActor::ShiftToRandomState()
{
    if (!PossibleStates.empty())
    {
        int RandomIndex = Math::RandRange(0, PossibleStates.size() - 1);
        ApplyQuantumState(PossibleStates[RandomIndex]);
    }
}

void AObservableActor::EnterFractureState()
{
    CurrentGlitchIntensity = 1.0f;
    ObservationState = EObservationState::Fractured;
}

void AObservableActor::UpdateQuantumState(float DeltaTime)
{
    EObservationState State = GetObservationState();

    if (State == EObservationState::Unobserved && bCanBeUnobserved)
    {
        if (Math::FRand() > 0.98f)
        {
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
            FVector GlitchOffset;
            GlitchOffset.X = std::sin(DeltaTime * 20.0f) * CurrentGlitchIntensity * 10.0f;
            GlitchOffset.Y = std::cos(DeltaTime * 20.0f) * CurrentGlitchIntensity * 10.0f;
            GlitchOffset.Z = std::sin(DeltaTime * 15.0f) * CurrentGlitchIntensity * 5.0f;
            Location = StoredLocation + GlitchOffset;
        }
    }
}

void AObservableActor::HandleObservationDecay(float DeltaTime)
{
    EObservationState State = GetObservationState();

    if (State == EObservationState::Observed)
    {
        CurrentObservationDuration += DeltaTime;
    }
    else if (State == EObservationState::Unobserved)
    {
        TimeSinceLastObserved += DeltaTime;
        if (TimeSinceLastObserved > ObservationDecayTime && bCanBeUnobserved)
        {
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

void AObservableActor::Print() const
{
    std::cout << "  - Observable Actor at (" << Location.X << ", " << Location.Y << ", " << Location.Z << ")" << std::endl;
}

}  // namespace ObservationParadox
