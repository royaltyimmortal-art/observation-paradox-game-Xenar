#include "Gameplay/GameState_Observation.h"
#include "Core/QuantumObservationSystem.h"
#include "World/RealityManager.h"

AGameState_Observation::AGameState_Observation()
    : CurrentAct(EGA_Awakening)
    , PlayerUnderstandingLevel(EPU_Confused)
    , GlobalCorruptionLevel(0.0f)
    , ParadoxLoopCount(0)
    , CorruptionIncreaseRate(0.01f)
    , MaxGlobalCorruption(1.0f)
    , TimeSinceActStart(0.0f)
{
}

void AGameState_Observation::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("Game started in Act: Awakening"));
    UpdateEnvironmentForAct();
}

void AGameState_Observation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    TimeSinceActStart += DeltaTime;
    UpdateActTransitions(DeltaTime);
    UpdateEnvironmentalEffects(DeltaTime);
    CheckEndingConditions();
}

void AGameState_Observation::ProgressToAct(EGameAct NewAct)
{
    if (NewAct != CurrentAct)
    {
        CurrentAct = NewAct;
        TimeSinceActStart = 0.0f;
        
        UE_LOG(LogTemp, Warning, TEXT("Progressed to act: %d"), (int32)CurrentAct);
        
        UpdateEnvironmentForAct();
    }
}

void AGameState_Observation::SetGlobalCorruption(float CorruptionLevel)
{
    GlobalCorruptionLevel = FMath::Clamp(CorruptionLevel, 0.0f, MaxGlobalCorruption);
}

void AGameState_Observation::IncreaseCorruption(float Amount)
{
    GlobalCorruptionLevel = FMath::Min(GlobalCorruptionLevel + Amount, MaxGlobalCorruption);
}

void AGameState_Observation::IncrementParadoxCounter()
{
    ParadoxLoopCount++;
    UE_LOG(LogTemp, Warning, TEXT("Paradox loop count: %d"), ParadoxLoopCount);
}

void AGameState_Observation::ResetParadoxCounter()
{
    ParadoxLoopCount = 0;
}

void AGameState_Observation::SetPlayerUnderstanding(EPlayerUnderstanding NewUnderstanding)
{
    if (NewUnderstanding != PlayerUnderstandingLevel)
    {
        PlayerUnderstandingLevel = NewUnderstanding;
        
        FString UnderstandingText[] = {
            TEXT("Confused"),
            TEXT("Notices Anomalies"),
            TEXT("Reality Shifts"),
            TEXT("Observation Effect"),
            TEXT("Paradox Awareness"),
            TEXT("Meta Awareness"),
            TEXT("Truth Revealed")
        };
        
        UE_LOG(LogTemp, Warning, TEXT("Player understanding: %s"), *UnderstandingText[(int32)NewUnderstanding]);
    }
}

void AGameState_Observation::RecordStoryEvent(const FString& EventDescription)
{
    StoryEvents.Add(EventDescription);
    UE_LOG(LogTemp, Warning, TEXT("Story Event: %s"), *EventDescription);
}

void AGameState_Observation::RevealTruth(const FString& TruthDescription)
{
    DiscoveredTruths.Add(TruthDescription, true);
    UE_LOG(LogTemp, Warning, TEXT("Truth Revealed: %s"), *TruthDescription);
}

bool AGameState_Observation::HasPlayerDiscoveredTruth(const FString& TruthID) const
{
    const bool* DiscoveredPtr = DiscoveredTruths.Find(TruthID);
    return DiscoveredPtr ? *DiscoveredPtr : false;
}

void AGameState_Observation::DetermineEndingPath()
{
    // Determine which ending the player will receive based on corruption and understanding
    if (GlobalCorruptionLevel > 0.8f && ParadoxLoopCount > 5)
    {
        TriggerFinalParadox();
    }
}

void AGameState_Observation::TriggerFinalParadox()
{
    ProgressToAct(EGA_EndingSequence);
    
    RecordStoryEvent(TEXT("The final paradox is triggered. The player realizes they are being observed."));
    SetPlayerUnderstanding(EPU_TruthRevealed);
}

void AGameState_Observation::QueueRadioMessage(const FString& Message, float Delay)
{
    // Queue a radio message to play after specified delay
    // "Observe anomalies. Do not lose continuity."
    UE_LOG(LogTemp, Warning, TEXT("Radio Message: %s"), *Message);
}

void AGameState_Observation::UpdateEnvironmentForAct()
{
    switch (CurrentAct)
    {
        case EGA_Awakening:
            // Act 1: Player wakes in NYC apartment
            // Start with normal reality
            SetGlobalCorruption(0.0f);
            QueueRadioMessage(TEXT("Observe anomalies. Do not lose continuity."), 5.0f);
            break;
            
        case EGA_Collapse:
            // Act 2: Reality becomes unstable
            // Increase corruption
            SetGlobalCorruption(0.3f);
            break;
            
        case EGA_Fracture:
            // Act 3: Sub-realities and backrooms
            // Significant corruption
            SetGlobalCorruption(0.6f);
            break;
            
        case EGA_Loop:
            // Act 4: Events repeat
            SetGlobalCorruption(0.8f);
            IncrementParadoxCounter();
            break;
            
        case EGA_ParadoxTower:
            // Final act: Paradox tower appears
            SetGlobalCorruption(1.0f);
            break;
            
        case EGA_EndingSequence:
            // Final truth sequence
            break;
            
        default:
            break;
    }
}

void AGameState_Observation::UpdateActTransitions(float DeltaTime)
{
    // Automatically progress through acts based on conditions
    switch (CurrentAct)
    {
        case EGA_Awakening:
            if (TimeSinceActStart > 180.0f)  // 3 minutes
            {
                ProgressToAct(EGA_Collapse);
            }
            break;
            
        case EGA_Collapse:
            if (GlobalCorruptionLevel > 0.5f)
            {
                ProgressToAct(EGA_Fracture);
            }
            break;
            
        case EGA_Fracture:
            if (ParadoxLoopCount > 2)
            {
                ProgressToAct(EGA_Loop);
            }
            break;
            
        case EGA_Loop:
            if (ParadoxLoopCount > 5)
            {
                ProgressToAct(EGA_ParadoxTower);
            }
            break;
            
        default:
            break;
    }
}

void AGameState_Observation::UpdateEnvironmentalEffects(float DeltaTime)
{
    // Gradually increase corruption over time
    IncreaseCorruption(CorruptionIncreaseRate * DeltaTime);
}

void AGameState_Observation::CheckEndingConditions()
{
    // Check if ending conditions are met
    if (GlobalCorruptionLevel >= MaxGlobalCorruption && CurrentAct == EGA_ParadoxTower)
    {
        TriggerFinalParadox();
    }
}
