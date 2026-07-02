#include "ObservationParadox.h"
#include "Core/QuantumObservationSystem.h"

#define LOCTEXT_NAMESPACE "FObservationParadoxModule"

void FObservationParadoxModule::StartupModule()
{
    // Initialize the Quantum Observation System
    FQuantumObservationSystem::Get().Initialize();
    UE_LOG(LogTemp, Warning, TEXT("Observation Paradox Module Started"));
}

void FObservationParadoxModule::ShutdownModule()
{
    // Shutdown systems
    FQuantumObservationSystem::Get().Shutdown();
    UE_LOG(LogTemp, Warning, TEXT("Observation Paradox Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE

FDEFINE_MODULE(FObservationParadoxModule, ObservationParadox)
