#ifndef QUANTUM_NPC_H
#define QUANTUM_NPC_H

#include "ObservableActor.h"

namespace ObservationParadox {

/**
 * QUANTUM NPC SYSTEM
 * NPCs that are aware of being observed
 */
class AQuantumNPC : public AObservableActor {
public:
    AQuantumNPC();
    virtual ~AQuantumNPC();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Observation response
    void RespondToObservation(float ObservationIntensity);
    void RespondToLostObservation();
    
    // State management
    void SetNPCState(ENPCState NewState);
    ENPCState GetNPCState() const { return CurrentState; }
    
    // Memory and history
    void AddMemory(const FNPCMemory& Memory);
    void ClearMemories() { Memories.clear(); }
    std::vector<FNPCMemory> GetMemories() const { return Memories; }
    
    // Dialogue system
    void PlayDialogue(const FDialogueNode& Dialogue);
    FDialogueNode GetContextualDialogue();
    
    // Alternative personalities
    void SwitchAlternativeVersion();
    void RegisterAlternativeVersion(const std::string& PersonalityID, const std::string& Description);
    
    // Fracture state
    void BeginFracture();
    void EndFracture();
    
    // Observation awareness
    bool IsBeingDirectlyObserved() const { return bIsDirectlyObserved; }
    float GetObservationIntensity() const { return CurrentObservationIntensity; }
    
    std::string GetName() const { return NPCName; }
    void SetName(const std::string& InName) { NPCName = InName; }
    
    virtual void Print() const override;
    
protected:
    ENPCState CurrentState;
    ENPCState PreviousState;
    
    bool bIsDirectlyObserved;
    float CurrentObservationIntensity;
    float ObservationDuration;
    
    std::vector<FNPCMemory> Memories;
    int MaxMemories;
    
    std::vector<FDialogueNode> DialogueOptions;
    FDialogueNode CurrentDialogue;
    
    std::map<std::string, std::string> AlternativeVersions;
    std::string CurrentPersonalityID;
    
    bool bIsFractured;
    std::vector<AQuantumNPC*> FractureInstances;
    
    float FreezeThreshold;
    float AlternativeVersionSwitchChance;
    bool bCanRememberImpossibleEvents;
    
    float StateTransitionTimer;
    FVector LastKnownPlayerLocation;
    
    std::string NPCName;
    
    virtual void UpdateNPCBehavior(float DeltaTime);
    virtual void HandleFrozenState(float DeltaTime);
    virtual void HandleGlitchingState(float DeltaTime);
    virtual void HandleFracturedState(float DeltaTime);
    virtual void DecayObservation(float DeltaTime);
};

}  // namespace ObservationParadox

#endif // QUANTUM_NPC_H
