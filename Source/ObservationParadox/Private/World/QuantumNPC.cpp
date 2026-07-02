#include "World/QuantumNPC.h"
#include "Core/QuantumObservationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

AQuantumNPC::AQuantumNPC()
    : CurrentState(ENS_Normal)
    , PreviousState(ENS_Normal)
    , bIsDirectlyObserved(false)
    , CurrentObservationIntensity(0.0f)
    , ObservationDuration(0.0f)
    , MaxMemories(50)
    , bIsFractured(false)
    , FreezeThreshold(0.7f)
    , AlternativeVersionSwitchChance(0.1f)
    , bCanRememberImpossibleEvents(true)
    , StateTransitionTimer(0.0f)
{
    PrimaryActorTick.TickInterval = 0.016f;
    
    // Disable default character movement while observed
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AQuantumNPC::BeginPlay()
{
    Super::BeginPlay();
    
    // Setup initial dialogue options
    if (DialogueOptions.Num() == 0)
    {
        FDialogueNode DefaultDialogue;
        DefaultDialogue.DialogueText = TEXT("Hello, I am an NPC.");
        DefaultDialogue.TriggerState = ENS_Normal;
        DefaultDialogue.ObservationThreshold = 0.0f;
        DefaultDialogue.bIsMetaCommentary = false;
        DialogueOptions.Add(DefaultDialogue);
    }
    
    // Initialize with default personality
    CurrentPersonalityID = TEXT("Default");
}

void AQuantumNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    UpdateNPCBehavior(DeltaTime);
}

void AQuantumNPC::RespondToObservation(float ObservationIntensity)
{
    bIsDirectlyObserved = true;
    CurrentObservationIntensity = ObservationIntensity;
    ObservationDuration = 0.0f;
    
    // If observation is intense enough, freeze the NPC
    if (ObservationIntensity > FreezeThreshold)
    {
        SetNPCState(ENS_Frozen);
        GetCharacterMovement()->MaxWalkSpeed = 0.0f;
    }
    else
    {
        SetNPCState(ENS_Aware);
        GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    }
}

void AQuantumNPC::RespondToLostObservation()
{
    bIsDirectlyObserved = false;
    CurrentObservationIntensity = 0.0f;
    SetNPCState(ENS_Normal);
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    
    // Maybe switch to alternative version
    if (FMath::FRand() < AlternativeVersionSwitchChance && AlternativeVersions.Num() > 0)
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
    if (Memories.Num() >= MaxMemories)
    {
        Memories.RemoveAt(0);  // Remove oldest memory
    }
    
    Memories.Add(Memory);
}

void AQuantumNPC::ClearMemories()
{
    Memories.Empty();
}

void AQuantumNPC::PlayDialogue(const FDialogueNode& Dialogue)
{
    CurrentDialogue = Dialogue;
    // In a real implementation, this would trigger voice lines and subtitles
    UE_LOG(LogTemp, Warning, TEXT("NPC Says: %s"), *Dialogue.DialogueText);
}

FDialogueNode AQuantumNPC::GetContextualDialogue()
{
    // Select dialogue based on current state and observation
    for (const FDialogueNode& Dialogue : DialogueOptions)
    {
        if (Dialogue.TriggerState == CurrentState && Dialogue.ObservationThreshold <= CurrentObservationIntensity)
        {
            return Dialogue;
        }
    }
    
    // Return default dialogue
    return DialogueOptions.Num() > 0 ? DialogueOptions[0] : FDialogueNode();
}

void AQuantumNPC::SwitchAlternativeVersion()
{
    if (AlternativeVersions.Num() == 0)
    {
        return;
    }
    
    // Pick random alternative personality
    TArray<FString> Keys;
    AlternativeVersions.GetKeys(Keys);
    
    if (Keys.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);
        CurrentPersonalityID = Keys[RandomIndex];
        
        // Maybe change appearance too
        if (FMath::FRand() > 0.5f)
        {
            // Trigger appearance change (would be implementation-specific)
        }
    }
}

void AQuantumNPC::RegisterAlternativeVersion(const FString& PersonalityID, const FString& Description)
{
    AlternativeVersions.Add(PersonalityID, Description);
}

void AQuantumNPC::BeginFracture()
{
    bIsFractured = true;
    SetNPCState(ENS_Fractured);
    
    // Create duplicate instances of this NPC
    for (int32 i = 0; i < 3; ++i)
    {
        AQuantumNPC* FractureInstance = GetWorld()->SpawnActor<AQuantumNPC>(
            GetClass(),
            GetActorLocation() + FVector(FMath::RandRange(-200.0f, 200.0f), FMath::RandRange(-200.0f, 200.0f), 0.0f),
            GetActorRotation()
        );
        
        if (FractureInstance)
        {
            FractureInstances.Add(FractureInstance);
        }
    }
}

void AQuantumNPC::EndFracture()
{
    bIsFractured = false;
    
    // Destroy fracture instances
    for (AQuantumNPC* Instance : FractureInstances)
    {
        if (Instance)
        {
            Instance->Destroy();
        }
    }
    
    FractureInstances.Empty();
    SetNPCState(ENS_Normal);
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
    if (CurrentState == ENS_Frozen)
    {
        // NPC is completely paralyzed
        GetCharacterMovement()->Velocity = FVector::ZeroVector;
    }
}

void AQuantumNPC::HandleGlitchingState(float DeltaTime)
{
    if (CurrentState == ENS_Glitching)
    {
        // Apply position glitches
        FVector GlitchOffset;
        GlitchOffset.X = FMath::Sin(GetWorld()->GetTimeSeconds() * 25.0f) * 20.0f;
        GlitchOffset.Y = FMath::Cos(GetWorld()->GetTimeSeconds() * 25.0f) * 20.0f;
        GlitchOffset.Z = FMath::Sin(GetWorld()->GetTimeSeconds() * 15.0f) * 10.0f;
        
        SetActorLocation(GetActorLocation() + GlitchOffset * DeltaTime);
    }
}

void AQuantumNPC::HandleFracturedState(float DeltaTime)
{
    if (CurrentState == ENS_Fractured)
    {
        // Multiple versions of NPC exist and move independently
        for (AQuantumNPC* Instance : FractureInstances)
        {
            if (Instance)
            {
                FVector RandomWalk = FVector(
                    FMath::FRandRange(-1.0f, 1.0f),
                    FMath::FRandRange(-1.0f, 1.0f),
                    0.0f
                ).GetSafeNormal() * 300.0f;
                
                Instance->GetCharacterMovement()->Velocity = RandomWalk;
            }
        }
    }
}

void AQuantumNPC::DecayObservation(float DeltaTime)
{
    if (bIsDirectlyObserved)
    {
        ObservationDuration += DeltaTime;
        CurrentObservationIntensity = FMath::Max(0.0f, CurrentObservationIntensity - DeltaTime * 0.2f);
    }
    else
    {
        ObservationDuration = 0.0f;
    }
}
