#include "Visuals/GlitchEffectSystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"

FGlitchEffectSystem::FGlitchEffectSystem()
    : CurrentGlitchIntensity(0.0f)
    , GlitchMaterial(nullptr)
    , ScreenDistortionMaterial(nullptr)
    , ChromaticMaterial(nullptr)
{
}

FGlitchEffectSystem::~FGlitchEffectSystem()
{
    Shutdown();
}

void FGlitchEffectSystem::Initialize()
{
    ConstructGlitchMaterials();
    UE_LOG(LogTemp, Warning, TEXT("Glitch Effect System Initialized"));
}

void FGlitchEffectSystem::Shutdown()
{
    GlitchLayers.Empty();
    ActiveGlitchTypes.Empty();
}

void FGlitchEffectSystem::Update(float DeltaTime)
{
    UpdateGlitchLayers(DeltaTime);
    ApplyGlitchToActors();
    UpdateMaterialParameters();
}

void FGlitchEffectSystem::TriggerGeometryWarp(AActor* TargetActor, float Intensity, float Duration)
{
    if (!TargetActor)
    {
        return;
    }
    
    FGlitchLayer Warp;
    Warp.GlitchType = EGT_GeometryWarp;
    Warp.Intensity = Intensity;
    Warp.Duration = Duration;
    Warp.FrequencyHz = 15.0f;
    Warp.GlitchAxis = FVector(1, 0, 0);
    Warp.bUseScreenSpace = false;
    
    GlitchLayers.Add(Warp);
    CurrentGlitchIntensity = FMath::Max(CurrentGlitchIntensity, Intensity);
}

void FGlitchEffectSystem::TriggerScreenDistortion(float Intensity, float Duration)
{
    FGlitchLayer Distortion;
    Distortion.GlitchType = EGT_ScreenTearing;
    Distortion.Intensity = Intensity;
    Distortion.Duration = Duration;
    Distortion.FrequencyHz = 20.0f;
    Distortion.bUseScreenSpace = true;
    
    GlitchLayers.Add(Distortion);
    CurrentGlitchIntensity = FMath::Max(CurrentGlitchIntensity, Intensity);
}

void FGlitchEffectSystem::TriggerChromaticAberration(float Intensity, float Duration)
{
    FGlitchLayer Chromatic;
    Chromatic.GlitchType = EGT_ChromaticShift;
    Chromatic.Intensity = Intensity;
    Chromatic.Duration = Duration;
    Chromatic.FrequencyHz = 10.0f;
    Chromatic.bUseScreenSpace = true;
    
    GlitchLayers.Add(Chromatic);
    CurrentGlitchIntensity = FMath::Max(CurrentGlitchIntensity, Intensity);
}

void FGlitchEffectSystem::TriggerShadowFlicker(AActor* TargetActor, float Frequency, float Duration)
{
    FGlitchLayer ShadowFlicker;
    ShadowFlicker.GlitchType = EGT_ShadowFlicker;
    ShadowFlicker.Intensity = 1.0f;
    ShadowFlicker.Duration = Duration;
    ShadowFlicker.FrequencyHz = Frequency;
    ShadowFlicker.bUseScreenSpace = false;
    
    GlitchLayers.Add(ShadowFlicker);
}

void FGlitchEffectSystem::TriggerTextureShift(AActor* TargetActor, float ShiftAmount, float Duration)
{
    FGlitchLayer TextureShift;
    TextureShift.GlitchType = EGT_TextureShift;
    TextureShift.Intensity = ShiftAmount;
    TextureShift.Duration = Duration;
    TextureShift.FrequencyHz = 5.0f;
    TextureShift.bUseScreenSpace = false;
    
    GlitchLayers.Add(TextureShift);
}

void FGlitchEffectSystem::TriggerMultipleGlitches(const TArray<FGlitchLayer>& Glitches)
{
    GlitchLayers.Append(Glitches);
    for (const FGlitchLayer& Glitch : Glitches)
    {
        CurrentGlitchIntensity = FMath::Max(CurrentGlitchIntensity, Glitch.Intensity);
    }
}

void FGlitchEffectSystem::ApplyCorruptionEffect(float CorruptionLevel)
{
    CurrentGlitchIntensity = CorruptionLevel;
    
    // Apply varied glitch effects based on corruption level
    if (CorruptionLevel > 0.3f)
    {
        TriggerScreenDistortion(CorruptionLevel * 0.5f, 0.1f);
    }
    if (CorruptionLevel > 0.6f)
    {
        TriggerChromaticAberration(CorruptionLevel * 0.3f, 0.1f);
    }
}

void FGlitchEffectSystem::ApplyFractureEffect(const FVector& FractureCenter, float Radius, float Intensity)
{
    // Create distortion centered at fracture point
    TriggerScreenDistortion(Intensity, 1.0f);
    TriggerGeometryWarp(nullptr, Intensity * 0.7f, 1.0f);
}

void FGlitchEffectSystem::ApplyNullZoneEffect(const FVector& NullZoneCenter, float Radius)
{
    // Create non-euclidean visual effects
    TriggerDepthDisruption(0.5f);
    TriggerScreenDistortion(0.3f, 0.5f);
}

void FGlitchEffectSystem::ApplyScreenWarp(float WarpAmount)
{
    FGlitchLayer Warp;
    Warp.GlitchType = EGT_ScreenTearing;
    Warp.Intensity = WarpAmount;
    Warp.Duration = 0.1f;
    Warp.bUseScreenSpace = true;
    
    GlitchLayers.Add(Warp);
}

void FGlitchEffectSystem::ApplyScreenTearing(int32 TearCount, float Intensity)
{
    for (int32 i = 0; i < TearCount; ++i)
    {
        FGlitchLayer Tear;
        Tear.GlitchType = EGT_ScreenTearing;
        Tear.Intensity = Intensity;
        Tear.Duration = 0.1f;
        Tear.bUseScreenSpace = true;
        
        GlitchLayers.Add(Tear);
    }
}

void FGlitchEffectSystem::ApplyDepthDisruption(float DisruptionAmount)
{
    FGlitchLayer DepthDisrupt;
    DepthDisrupt.GlitchType = EGT_DepthDisruption;
    DepthDisrupt.Intensity = DisruptionAmount;
    DepthDisrupt.Duration = 0.5f;
    DepthDisrupt.bUseScreenSpace = true;
    
    GlitchLayers.Add(DepthDisrupt);
}

void FGlitchEffectSystem::SetGlitchMaterialParameter(const FName& ParameterName, float Value)
{
    if (GlitchMaterial)
    {
        GlitchMaterial->SetScalarParameterValue(ParameterName, Value);
    }
}

void FGlitchEffectSystem::SetGlitchMaterialParameter(const FName& ParameterName, const FVector& Value)
{
    if (GlitchMaterial)
    {
        GlitchMaterial->SetVectorParameterValue(ParameterName, FLinearColor(Value.X, Value.Y, Value.Z));
    }
}

void FGlitchEffectSystem::SetGlitchMaterialParameter(const FName& ParameterName, UTexture* Texture)
{
    if (GlitchMaterial && Texture)
    {
        GlitchMaterial->SetTextureParameterValue(ParameterName, Texture);
    }
}

void FGlitchEffectSystem::UpdateGlitchLayers(float DeltaTime)
{
    // Update and remove expired glitch layers
    for (int32 i = GlitchLayers.Num() - 1; i >= 0; --i)
    {
        GlitchLayers[i].Duration -= DeltaTime;
        if (GlitchLayers[i].Duration <= 0.0f)
        {
            GlitchLayers.RemoveAt(i);
        }
    }
    
    // Decay overall glitch intensity
    if (GlitchLayers.Num() == 0)
    {
        CurrentGlitchIntensity = FMath::Max(CurrentGlitchIntensity - DeltaTime * 0.5f, 0.0f);
    }
    else
    {
        CurrentGlitchIntensity = 0.0f;
        for (const FGlitchLayer& Layer : GlitchLayers)
        {
            CurrentGlitchIntensity = FMath::Max(CurrentGlitchIntensity, Layer.Intensity);
        }
    }
}

void FGlitchEffectSystem::ApplyGlitchToActors()
{
    // Apply glitch effects to visible actors
}

void FGlitchEffectSystem::UpdateMaterialParameters()
{
    if (GlitchMaterial)
    {
        // Update shader parameters based on current glitches
        GlitchMaterial->SetScalarParameterValue(TEXT("GlitchIntensity"), CurrentGlitchIntensity);
        GlitchMaterial->SetScalarParameterValue(TEXT("Time"), GetWorld()->GetTimeSeconds());
    }
}

void FGlitchEffectSystem::ConstructGlitchMaterials()
{
    // Load or create material instances for glitch effects
    // In production, these would reference actual Unreal materials
}
