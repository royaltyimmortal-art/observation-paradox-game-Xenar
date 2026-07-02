#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Sound/SoundBase.h"
#include "QuantumAudioSystem.generated.h"

/**
 * QUANTUM AUDIO SYSTEM
 * 
 * Advanced 3D audio that responds to observation state:
 * - Sirens that may not actually exist (quantum uncertainty)
 * - Whispered audio hidden in ambient noise
 * - Sound that changes based on attention focus
 * - Non-euclidean audio (sounds from impossible directions)
 * - Time-distorted audio (pitch changes with reality corruption)
 * - Psychoacoustic effects for psychological horror
 */

ENUM(uint8, EAudioReality)
{
    EAR_Real,            // Actually happening
    EAR_Uncertain,       // Might be real or psychological
    EAR_Psychological,   // Only in player's mind
    EAR_Distorted,       // Warped by reality corruption
    EAR_Impossible       // Coming from a direction that's geometrically impossible
};

STRUCT_TYPE(FAmbientAudioLayer)
{
    GENERATED_BODY()

    USoundBase* AudioSource;
    float Volume;
    float FrequencyShift;  // For pitch modulation
    bool bLoops;
    EAudioReality AudioReality;
    float PanAmount;  // Stereo panning
    float ReverbAmount;
};

STRUCT_TYPE(FQuantumAudioEvent)
{
    GENERATED_BODY()

    FVector AudioLocation;
    USoundBase* Sound;
    float Volume;
    float MaxDistance;
    float MinDistance;
    EAudioReality AudioReality;
    bool bCanBeSilenced;  // By observation
    float Probability;  // Chance this sound actually plays
};

UCLASS()
class OBSERVATIONPARADOX_API UQuantumAudioSystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UQuantumAudioSystem();
    
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    
    // Ambient audio setup
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void SetupAmbientAudioLayers(const TArray<FAmbientAudioLayer>& Layers);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void UpdateAmbientAudioIntensity(float Intensity);
    
    // Audio events
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void PlayQuantumAudio(const FQuantumAudioEvent& AudioEvent);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void PlayUncertainAudio(const FVector& Location, USoundBase* Sound, float Probability);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void PlayPsychologicalAudio(USoundBase* Sound, float SanityFactor);
    
    // Whisper system
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void AddWhisperToAmbience(const FString& WhisperText, float Volume);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void UpdateWhispers(float DeltaTime);
    
    // Audio distortion
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void ApplyAudioDistortion(float CorruptionAmount);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void ModifyAudioPitch(USoundBase* Sound, float PitchMultiplier);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void ModifyAudioReverb(float ReverbAmount);
    
    // Spatial audio
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void PlayImpossibleAudio(const FVector& ImpossibleDirection);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void PlayNonEuclideanAudio(const FVector& LocationA, const FVector& LocationB);
    
    // Environmental response
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void SilenceAudioOnObservation(USoundBase* Sound);
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void UnsilenceAudioWhenUnobserved(USoundBase* Sound);
    
    // Reality zone effects
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void ApplyStableZoneAudio();
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void ApplyFractureZoneAudio();
    
    UFUNCTION(BlueprintCallable, Category = "QuantumAudio")
    void ApplyNullZoneAudio();
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "QuantumAudio")
    TArray<FAmbientAudioLayer> AmbientLayers;
    
    UPROPERTY(BlueprintReadOnly, Category = "QuantumAudio")
    TArray<FQuantumAudioEvent> ActiveAudioEvents;
    
    UPROPERTY(BlueprintReadOnly, Category = "QuantumAudio")
    TArray<FString> WhisperQueue;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuantumAudio")
    float GlobalAudioVolume;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuantumAudio")
    float DistortionAmount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuantumAudio")
    float ReverbAmount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuantumAudio")
    float AmbientNoiseLevel;
    
    virtual void UpdateAmbientAudio(float DeltaTime);
    virtual void ProcessAudioEvents(float DeltaTime);
    virtual void ApplyObservationEffects();
    virtual void UpdateAudioEnvironment(float DeltaTime);
    
private:
    float CurrentCorruptionLevel;
    float CurrentSanity;
};
