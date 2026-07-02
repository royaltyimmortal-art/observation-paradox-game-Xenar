#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "ObservationParadox.h"

namespace ObservationParadox {

/**
 * GAME STATE - OBSERVATION PARADOX
 * Tracks global game state and story progression
 */
class AGameState {
public:
    AGameState();
    ~AGameState();
    
    void Initialize();
    void Update(float DeltaTime);
    
    // Act progression
    void ProgressToAct(EGameAct NewAct);
    EGameAct GetCurrentAct() const { return CurrentAct; }
    
    // Reality corruption
    void SetGlobalCorruption(float CorruptionLevel);
    float GetGlobalCorruption() const { return GlobalCorruptionLevel; }
    void IncreaseCorruption(float Amount);
    
    // Paradox mechanics
    void IncrementParadoxCounter();
    int GetParadoxCount() const { return ParadoxLoopCount; }
    void ResetParadoxCounter();
    
    // Player understanding tracking
    void SetPlayerUnderstanding(EPlayerUnderstanding NewUnderstanding);
    EPlayerUnderstanding GetPlayerUnderstanding() const { return PlayerUnderstandingLevel; }
    
    // Story events
    void RecordStoryEvent(const std::string& EventDescription);
    std::vector<std::string> GetStoryEvents() const { return StoryEvents; }
    
    // Major revelations
    void RevealTruth(const std::string& TruthDescription);
    bool HasPlayerDiscoveredTruth(const std::string& TruthID) const;
    
    // Ending determination
    void DetermineEndingPath();
    void TriggerFinalParadox();
    
    // Singleton access
    static AGameState& Get();
    
    void Print() const;
    
private:
    EGameAct CurrentAct;
    EPlayerUnderstanding PlayerUnderstandingLevel;
    
    float GlobalCorruptionLevel;
    int ParadoxLoopCount;
    
    std::vector<std::string> StoryEvents;
    std::map<std::string, bool> DiscoveredTruths;
    
    float CorruptionIncreaseRate;
    float MaxGlobalCorruption;
    
    float TimeSinceActStart;
    
    void UpdateActTransitions(float DeltaTime);
    void UpdateEnvironmentalEffects(float DeltaTime);
    void CheckEndingConditions();
};

}  // namespace ObservationParadox

#endif // GAME_STATE_H
