#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Core/QuantumObservationSystem.h"
#include "GameState_Observation.generated.h"

/**
 * GAME STATE - OBSERVATION PARADOX
 * 
 * Tracks global game state including:
 * - Current act and story progression
 * - Reality corruption level (global)
 * - Paradox counter (how many times has player looped?)
 * - Player's understanding of the truth
 * - Major story events and revelations
 * - Ending condition determination
 */

ENUM(uint8, EGameAct)
{
    EGA_Awakening,        // Act 1: Player wakes in NYC apartment
    EGA_Collapse,         // Act 2: Reality becomes unstable
    EGA_Fracture,         // Act 3: Sub-realities and backrooms
    EGA_Loop,             // Act 4: Events repeat with variations
    EGA_ParadoxTower,     // Final act: Hidden structure
    EGA_EndingSequence    // Final truth sequence
};

ENUM(uint8, EPlayerUnderstanding)
{
    EPU_Confused,         // "What's happening?"
    EPU_NoticesAnomalies, // "Something is wrong"
    EPU_RealityShifts,    // "The world changes when I'm not looking"
    EPU_ObservationEffect, // "My observation creates reality"
    EPU_ParadoxAwareness, // "Reality has no base state"
    EPU_MetaAwareness,    // "I am being observed"
    EPU_TruthRevealed     // "There is no escape"
};

UCLASS()
class OBSERVATIONPARADOX_API AGameState_Observation : public AGameState
{
    GENERATED_BODY()

public:
    AGameState_Observation();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Act progression
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void ProgressToAct(EGameAct NewAct);
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    EGameAct GetCurrentAct() const { return CurrentAct; }
    
    // Reality corruption
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void SetGlobalCorruption(float CorruptionLevel);
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    float GetGlobalCorruption() const { return GlobalCorruptionLevel; }
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void IncreaseCorruption(float Amount);
    
    // Paradox mechanics
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void IncrementParadoxCounter();
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    int32 GetParadoxCount() const { return ParadoxLoopCount; }
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void ResetParadoxCounter();
    
    // Player understanding tracking
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void SetPlayerUnderstanding(EPlayerUnderstanding NewUnderstanding);
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    EPlayerUnderstanding GetPlayerUnderstanding() const { return PlayerUnderstandingLevel; }
    
    // Story events
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void RecordStoryEvent(const FString& EventDescription);
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    TArray<FString> GetStoryEvents() const { return StoryEvents; }
    
    // Major revelations
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void RevealTruth(const FString& TruthDescription);
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    bool HasPlayerDiscoveredTruth(const FString& TruthID) const;
    
    // Ending determination
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void DetermineEndingPath();
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void TriggerFinalParadox();
    
    // Environmental storytelling
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void QueueRadioMessage(const FString& Message, float Delay);
    
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void UpdateEnvironmentForAct();
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    EGameAct CurrentAct;
    
    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    EPlayerUnderstanding PlayerUnderstandingLevel;
    
    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    float GlobalCorruptionLevel;
    
    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    int32 ParadoxLoopCount;
    
    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    TArray<FString> StoryEvents;
    
    UPROPERTY(BlueprintReadOnly, Category = "GameState")
    TMap<FString, bool> DiscoveredTruths;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
    float CorruptionIncreaseRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
    float MaxGlobalCorruption;
    
    virtual void UpdateActTransitions(float DeltaTime);
    virtual void UpdateEnvironmentalEffects(float DeltaTime);
    virtual void CheckEndingConditions();
    
private:
    float TimeSinceActStart;
    FTimerHandle EndingCheckTimer;
};
