#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/ObservableActor.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "QuantumNPC.generated.h"

/**
 * QUANTUM NPC SYSTEM
 * 
 * NPCs that are aware of being observed and behave differently based on observation state:
 * - Freeze when directly observed
 * - Change dialogue based on observation intensity
 * - Remember impossible things that happened when unobserved
 * - Say disturbing meta-commentary about observation
 * - Shift between different versions of themselves
 * - Respond to player's perception of them
 */

ENUM(uint8, ENPCState)
{
    ENS_Normal,          // Regular behavior
    ENS_Aware,           // Knows it's being observed
    ENS_Frozen,          // Completely paralyzed by observation
    ENS_Glitching,       // Reality distortion around NPC
    ENS_Alternative,     // Different personality/appearance
    ENS_Fractured,       // Multiple versions overlapping
    ENS_Unraveling       // Breaking down reality around self
};

STRUCT_TYPE(FNPCMemory)
{
    GENERATED_BODY()

    FString Description;
    float TimeSinceEvent;
    bool bIsImpossible;  // Memory of something that couldn't have happened
    float Probability;   // How real this memory feels
};

STRUCT_TYPE(FDialogueNode)
{
    GENERATED_BODY()

    FString DialogueText;
    ENPCState TriggerState;
    float ObservationThreshold;
    bool bIsMetaCommentary;  // Breaking the fourth wall about observation
    FString AudioID;
};

UCLASS()
class OBSERVATIONPARADOX_API AQuantumNPC : public ACharacter
{
    GENERATED_BODY()

public:
    AQuantumNPC();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Observation response
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void RespondToObservation(float ObservationIntensity);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void RespondToLostObservation();
    
    // State management
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void SetNPCState(ENPCState NewState);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    ENPCState GetNPCState() const { return CurrentState; }
    
    // Memory and history
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void AddMemory(const FNPCMemory& Memory);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void ClearMemories();
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    TArray<FNPCMemory> GetMemories() const { return Memories; }
    
    // Dialogue system
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void PlayDialogue(const FDialogueNode& Dialogue);
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    FDialogueNode GetContextualDialogue();
    
    // Alternative personalities
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void SwitchAlternativeVersion();
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void RegisterAlternativeVersion(const FString& PersonalityID, const FString& Description);
    
    // Fracture state
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void BeginFracture();
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    void EndFracture();
    
    // Observation awareness
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    bool IsBeingDirectlyObserved() const { return bIsDirectlyObserved; }
    
    UFUNCTION(BlueprintCallable, Category = "Quantum")
    float GetObservationIntensity() const { return CurrentObservationIntensity; }
    
protected:
    // State
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    ENPCState CurrentState;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    ENPCState PreviousState;
    
    // Observation tracking
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    bool bIsDirectlyObserved;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    float CurrentObservationIntensity;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    float ObservationDuration;
    
    // Memory system
    UPROPERTY(BlueprintReadWrite, Category = "Quantum")
    TArray<FNPCMemory> Memories;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    int32 MaxMemories;
    
    // Dialogue
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    TArray<FDialogueNode> DialogueOptions;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    FDialogueNode CurrentDialogue;
    
    // Alternative versions
    UPROPERTY(BlueprintReadWrite, Category = "Quantum")
    TMap<FString, FString> AlternativeVersions;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    FString CurrentPersonalityID;
    
    // Fracture state
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    bool bIsFractured;
    
    UPROPERTY(BlueprintReadOnly, Category = "Quantum")
    TArray<AQuantumNPC*> FractureInstances;
    
    // Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    float FreezeThreshold;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    float AlternativeVersionSwitchChance;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quantum")
    bool bCanRememberImpossibleEvents;
    
    virtual void UpdateNPCBehavior(float DeltaTime);
    virtual void HandleFrozenState(float DeltaTime);
    virtual void HandleGlitchingState(float DeltaTime);
    virtual void HandleFracturedState(float DeltaTime);
    virtual void DecayObservation(float DeltaTime);
    
private:
    float StateTransitionTimer;
    FVector LastKnownPlayerLocation;
};
