#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GlitchEffectSystem.generated.h"

/**
 * GLITCH EFFECT SYSTEM
 * 
 * Visual effects for reality corruption and quantum uncertainty:
 * - Geometry breathing and warping
 * - Flicker shadows and light anomalies
 * - Chromatic aberration (color separation)
 * - Screen distortion and warping
 * - Model clipping and Z-fighting effects
 * - Temporal artifacts and motion blur corruption
 * - Shader-based reality corruption
 */

ENUM(uint8, EGlitchType)
{
    EGT_GeometryWarp,         // Vertices move incorrectly
    EGT_ChromaticShift,       // Color channels separate
    EGT_ScreenTearing,        // Horizontal scan lines
    EGT_MotionBlurCorruption,  // Blur artifacts
    EGT_ShadowFlicker,        // Shadows break and reform
    EGT_TextureShift,         // Textures shift position
    EGT_DepthDisruption,      // Z-buffer corruption visual
    EGT_TimeDistortion        // Motion trails
};

STRUCT_TYPE(FGlitchLayer)
{
    GENERATED_BODY()

    EGlitchType GlitchType;
    float Intensity;        // 0.0 to 1.0
    float Duration;         // How long this glitch persists
    float FrequencyHz;      // How fast it pulses
    FVector GlitchAxis;     // Direction of distortion
    bool bUseScreenSpace;   // Apply in screen space vs world space
};

/**
 * Non-subsystem glitch manager for performance
 */
class OBSERVATIONPARADOX_API FGlitchEffectSystem
{
public:
    FGlitchEffectSystem();
    ~FGlitchEffectSystem();
    
    void Initialize();
    void Shutdown();
    void Update(float DeltaTime);
    
    // Trigger glitches
    void TriggerGeometryWarp(AActor* TargetActor, float Intensity, float Duration);
    void TriggerScreenDistortion(float Intensity, float Duration);
    void TriggerChromaticAberration(float Intensity, float Duration);
    void TriggerShadowFlicker(AActor* TargetActor, float Frequency, float Duration);
    void TriggerTextureShift(AActor* TargetActor, float ShiftAmount, float Duration);
    void TriggerMultipleGlitches(const TArray<FGlitchLayer>& Glitches);
    
    // Global effects
    void ApplyCorruptionEffect(float CorruptionLevel);  // 0.0 to 1.0
    void ApplyFractureEffect(const FVector& FractureCenter, float Radius, float Intensity);
    void ApplyNullZoneEffect(const FVector& NullZoneCenter, float Radius);
    
    // Screen effects
    void ApplyScreenWarp(float WarpAmount);
    void ApplyScreenTearing(int32 TearCount, float Intensity);
    void ApplyDepthDisruption(float DisruptionAmount);
    
    // Material parameter control
    void SetGlitchMaterialParameter(const FName& ParameterName, float Value);
    void SetGlitchMaterialParameter(const FName& ParameterName, const FVector& Value);
    void SetGlitchMaterialParameter(const FName& ParameterName, UTexture* Texture);
    
    // Query current glitch state
    float GetCurrentGlitchIntensity() const { return CurrentGlitchIntensity; }
    bool IsGlitching() const { return CurrentGlitchIntensity > 0.0f; }
    TArray<EGlitchType> GetActiveGlitchTypes() const { return ActiveGlitchTypes; }
    
private:
    float CurrentGlitchIntensity;
    TArray<EGlitchType> ActiveGlitchTypes;
    TArray<FGlitchLayer> GlitchLayers;
    
    // Material instances for glitch effects
    class UMaterialInstanceDynamic* GlitchMaterial;
    class UMaterialInstanceDynamic* ScreenDistortionMaterial;
    class UMaterialInstanceDynamic* ChromaticMaterial;
    
    void UpdateGlitchLayers(float DeltaTime);
    void ApplyGlitchToActors();
    void UpdateMaterialParameters();
    void ConstructGlitchMaterials();
};
