#include "GameState.h"
#include <iostream>

namespace ObservationParadox {

static AGameState* GGameState = nullptr;

AGameState::AGameState()
    : CurrentAct(EGameAct::Awakening)
    , PlayerUnderstandingLevel(EPlayerUnderstanding::Confused)
    , GlobalCorruptionLevel(0.0f)
    , ParadoxLoopCount(0)
    , CorruptionIncreaseRate(0.01f)
    , MaxGlobalCorruption(1.0f)
    , TimeSinceActStart(0.0f)
{
}

AGameState::~AGameState()
{
}

void AGameState::Initialize()
{
    std::cout << "\n" << std::endl;
    std::cout << "╔════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           OBSERVATION: PARADOX - GAME STATE INITIALIZED         ║" << std::endl;
    std::cout << "║  Reality is NOT fixed. Reality collapses based on OBSERVATION.  ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
}

void AGameState::Update(float DeltaTime)
{
    TimeSinceActStart += DeltaTime;
    UpdateActTransitions(DeltaTime);
    UpdateEnvironmentalEffects(DeltaTime);
    CheckEndingConditions();
}

void AGameState::ProgressToAct(EGameAct NewAct)
{
    if (NewAct != CurrentAct)
    {
        CurrentAct = NewAct;
        TimeSinceActStart = 0.0f;

        std::string ActNames[] = {"Awakening", "Collapse", "Fracture", "Loop", "Paradox Tower", "Ending Sequence"};
        std::cout << "\n[GAME STATE] Progressed to Act: " << ActNames[(int)CurrentAct] << std::endl;
    }
}

void AGameState::SetGlobalCorruption(float CorruptionLevel)
{
    GlobalCorruptionLevel = Math::Clamp(CorruptionLevel, 0.0f, MaxGlobalCorruption);
}

void AGameState::IncreaseCorruption(float Amount)
{
    GlobalCorruptionLevel = Math::Clamp(GlobalCorruptionLevel + Amount, 0.0f, MaxGlobalCorruption);
}

void AGameState::IncrementParadoxCounter()
{
    ParadoxLoopCount++;
    std::cout << "[GAME STATE] Paradox loop count: " << ParadoxLoopCount << std::endl;
}

void AGameState::ResetParadoxCounter()
{
    ParadoxLoopCount = 0;
}

void AGameState::SetPlayerUnderstanding(EPlayerUnderstanding NewUnderstanding)
{
    if (NewUnderstanding != PlayerUnderstandingLevel)
    {
        PlayerUnderstandingLevel = NewUnderstanding;
        std::string UnderstandingText[] = {
            "Confused",
            "Notices Anomalies",
            "Reality Shifts",
            "Observation Effect",
            "Paradox Awareness",
            "Meta Awareness",
            "Truth Revealed"
        };
        std::cout << "[GAME STATE] Player understanding: " << UnderstandingText[(int)NewUnderstanding] << std::endl;
    }
}

void AGameState::RecordStoryEvent(const std::string& EventDescription)
{
    StoryEvents.push_back(EventDescription);
    std::cout << "[GAME STATE] Story Event: " << EventDescription << std::endl;
}

void AGameState::RevealTruth(const std::string& TruthDescription)
{
    DiscoveredTruths[TruthDescription] = true;
    std::cout << "[GAME STATE] Truth Revealed: " << TruthDescription << std::endl;
}

bool AGameState::HasPlayerDiscoveredTruth(const std::string& TruthID) const
{
    auto it = DiscoveredTruths.find(TruthID);
    return it != DiscoveredTruths.end() ? it->second : false;
}

void AGameState::DetermineEndingPath()
{
    if (GlobalCorruptionLevel > 0.8f && ParadoxLoopCount > 5)
    {
        TriggerFinalParadox();
    }
}

void AGameState::TriggerFinalParadox()
{
    ProgressToAct(EGameAct::EndingSequence);
    RecordStoryEvent("The final paradox is triggered. The player realizes they are being observed.");
    SetPlayerUnderstanding(EPlayerUnderstanding::TruthRevealed);
}

AGameState& AGameState::Get()
{
    if (!GGameState)
        GGameState = new AGameState();
    return *GGameState;
}

void AGameState::UpdateActTransitions(float DeltaTime)
{
    switch (CurrentAct)
    {
        case EGameAct::Awakening:
            if (TimeSinceActStart > 10.0f)
                ProgressToAct(EGameAct::Collapse);
            break;
        case EGameAct::Collapse:
            if (GlobalCorruptionLevel > 0.3f)
                ProgressToAct(EGameAct::Fracture);
            break;
        case EGameAct::Fracture:
            if (ParadoxLoopCount > 2)
                ProgressToAct(EGameAct::Loop);
            break;
        case EGameAct::Loop:
            if (ParadoxLoopCount > 5)
                ProgressToAct(EGameAct::ParadoxTower);
            break;
        default:
            break;
    }
}

void AGameState::UpdateEnvironmentalEffects(float DeltaTime)
{
    IncreaseCorruption(CorruptionIncreaseRate * DeltaTime);
}

void AGameState::CheckEndingConditions()
{
    if (GlobalCorruptionLevel >= MaxGlobalCorruption && CurrentAct == EGameAct::ParadoxTower)
    {
        TriggerFinalParadox();
    }
}

void AGameState::Print() const
{
    std::cout << "\n═════════════════════════════════════════════════════════" << std::endl;
    std::cout << "  GAME STATE" << std::endl;
    std::cout << "═════════════════════════════════════════════════════════" << std::endl;
    std::cout << "  Current Act: " << (int)CurrentAct << std::endl;
    std::cout << "  Global Corruption: " << GlobalCorruptionLevel * 100.0f << "%" << std::endl;
    std::cout << "  Paradox Loops: " << ParadoxLoopCount << std::endl;
    std::cout << "  Player Understanding: " << (int)PlayerUnderstandingLevel << std::endl;
    std::cout << "  Story Events: " << StoryEvents.size() << std::endl;
    std::cout << "═════════════════════════════════════════════════════════" << std::endl;
}

}  // namespace ObservationParadox
