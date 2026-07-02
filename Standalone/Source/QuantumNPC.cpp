#include "QuantumNPC.h"
#include "QuantumObservationSystem.h"
#include <iostream>
#include <algorithm>

namespace ObservationParadox {

AQuantumNPC::AQuantumNPC()
    : CurrentState(ENPCState::Normal)
    , PreviousState(ENPCState::Normal)
    , bIsDirectlyObserved(false)
    , CurrentObservationIntensity(0.0f)
    , ObservationDuration(0.0f)
    , MaxMemories(50)
    , bIsFractured(false)
    , FreezeThreshold(0.7f)
    , AlternativeVersionSwitchChance(0.1f)
    , bCanRememberImpossibleEvents(true)
    , StateTransitionTimer(0.0f)
    , NPCName("NPC")
{
}

AQuantumNPC::~AQuantumNPC()
{
}

void AQuantumNPC::BeginPlay()
{
    AObservableActor::BeginPlay();

    if (DialogueOptions.empty())
    {
        FDialogueNode DefaultDialogue;
        DefaultDialogue.DialogueText = "Hello, I am an NPC.";
        DefaultDialogue.TriggerState = ENPCState::Normal;
        DefaultDialogue.ObservationThreshold = 0.0f;
        DefaultDialogue.bIsMetaCommentary = false;
        DialogueOptions.push_back(DefaultDialogue);
    }

    CurrentPersonalityID = "Default";
}

void AQuantumNPC::Tick(float DeltaTime)
{
    AObservableActor::Tick(DeltaTime);
    UpdateNPCBehavior(DeltaTime);
}

void AQuantumNPC::RespondToObservation(float ObservationIntensity)
{
    bIsDirectlyObserved = true;
    CurrentObservationIntensity = ObservationIntensity;
    ObservationDuration = 0.0f;

    if (ObservationIntensity > FreezeThreshold)
    {
        SetNPCState(ENPCState::Frozen);
    }
    else
    {
        SetNPCState(ENPCState::Aware);
    }
}

void AQuantumNPC::RespondToLostObservation()
{
    bIsDirectlyObserved = false;
    CurrentObservationIntensity = 0.0f;
    SetNPCState(ENPCState::Normal);

    if (Math::FRand() < AlternativeVersionSwitchChance && !AlternativeVersions.empty())
    {
        SwitchAlternativeVersion();
    }
}

void AQuantumNPC::SetNPCState(ENPCState NewState)
{
    if (NewState != CurrentState)
    {
        PreviousState = CurrentState;
        CurrentState = NewState;
        StateTransitionTimer = 0.0f;
    }
}

void AQuantumNPC::AddMemory(const FNPCMemory& Memory)
{
    if (Memories.size() >= MaxMemories)
    {
        Memories.erase(Memories.begin());
    }
    Memories.push_back(Memory);
}

void AQuantumNPC::PlayDialogue(const FDialogueNode& Dialogue)
{
    CurrentDialogue = Dialogue;
    std::cout << "[" << NPCName << "]: " << Dialogue.DialogueText << std::endl;
}

FDialogueNode AQuantumNPC::GetContextualDialogue()
{
    for (const FDialogueNode& Dialogue : DialogueOptions)
    {
        if (Dialogue.TriggerState == CurrentState && Dialogue.ObservationThreshold <= CurrentObservationIntensity)
        {
            return Dialogue;
        }
    }
    return DialogueOptions.empty() ? FDialogueNode() : DialogueOptions[0];
}

void AQuantumNPC::SwitchAlternativeVersion()
{
    if (AlternativeVersions.empty()) return;

    std::vector<std::string> Keys;
    for (const auto& Pair : AlternativeVersions)
    {
        Keys.push_back(Pair.first);
    }

    if (!Keys.empty())
    {
        int RandomIndex = Math::RandRange(0, Keys.size() - 1);
        CurrentPersonalityID = Keys[RandomIndex];
    }
}

void AQuantumNPC::RegisterAlternativeVersion(const std::string& PersonalityID, const std::string& Description)
{
    AlternativeVersions[PersonalityID] = Description;
}

void AQuantumNPC::BeginFracture()
{
    bIsFractured = true;
    SetNPCState(ENPCState::Fractured);

    for (int i = 0; i < 3; ++i)
    {
        AQuantumNPC* FractureInstance = new AQuantumNPC();
        FractureInstance->SetActorLocation(Location + FVector(Math::FRandRange(-200.0f, 200.0f), Math::FRandRange(-200.0f, 200.0f), 0.0f));
        FractureInstances.push_back(FractureInstance);
    }
}

void AQuantumNPC::EndFracture()
{
    bIsFractured = false;

    for (AQuantumNPC* Instance : FractureInstances)
    {
        if (Instance)
            delete Instance;
    }

    FractureInstances.clear();
    SetNPCState(ENPCState::Normal);
}

void AQuantumNPC::UpdateNPCBehavior(float DeltaTime)
{
    StateTransitionTimer += DeltaTime;
    HandleFrozenState(DeltaTime);
    HandleGlitchingState(DeltaTime);
    HandleFracturedState(DeltaTime);
    DecayObservation(DeltaTime);
}

void AQuantumNPC::HandleFrozenState(float DeltaTime)
{
    if (CurrentState == ENPCState::Frozen)
    {
        // NPC is paralyzed
    }
}

void AQuantumNPC::HandleGlitchingState(float DeltaTime)
{
    if (CurrentState == ENPCState::Glitching)
    {
        FVector GlitchOffset;
        GlitchOffset.X = std::sin(DeltaTime * 25.0f) * 20.0f;
        GlitchOffset.Y = std::cos(DeltaTime * 25.0f) * 20.0f;
        GlitchOffset.Z = std::sin(DeltaTime * 15.0f) * 10.0f;
        Location = Location + (GlitchOffset * DeltaTime);
    }
}

void AQuantumNPC::HandleFracturedState(float DeltaTime)
{
    if (CurrentState == ENPCState::Fractured)
    {
        for (AQuantumNPC* Instance : FractureInstances)
        {
            if (Instance)
            {
                FVector RandomWalk = FVector(Math::FRandRange(-1.0f, 1.0f), Math::FRandRange(-1.0f, 1.0f), 0.0f).GetNormalized() * 300.0f;
            }
        }
    }
}

void AQuantumNPC::DecayObservation(float DeltaTime)
{
    if (bIsDirectlyObserved)
    {
        ObservationDuration += DeltaTime;
        CurrentObservationIntensity = Math::Clamp(CurrentObservationIntensity - DeltaTime * 0.2f, 0.0f, 1.0f);
    }
    else
    {
        ObservationDuration = 0.0f;
    }
}

void AQuantumNPC::Print() const
{
    std::cout << "  [NPC] " << NPCName << " at (" << Location.X << ", " << Location.Y << ", " << Location.Z << ") ";
    std::cout << "State: " << (int)CurrentState << std::endl;
}

}  // namespace ObservationParadox
