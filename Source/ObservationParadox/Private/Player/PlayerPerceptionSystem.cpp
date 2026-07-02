#include "Player/PlayerPerceptionSystem.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Core/QuantumObservationSystem.h"

UPlayerPerceptionSystem::UPlayerPerceptionSystem()
    : PrimaryFocusIntensity(0.0f)
    , PrimaryFocusDuration(0.0f)
    , bHasNoticedAnomaly(false)
    , AnomalyAwareness(0.0f)
    , AnomalyDetectionThreshold(0.5f)
    , AttentionCapacity(1.0f)
    , CurrentAttentionLoad(0.0f)
    , MaxAttentionCapacity(1.0f)
    , AttentionRecoveryRate(0.1f)
    , CurrentSanity(100.0f)
    , MaxSanity(100.0f)
    , SanityRecoveryRate(0.05f)
    , FieldOfViewAngle(90.0f)
    , MaxPerceptionDistance(5000.0f)
    , PeripheralVisionAngle(135.0f)
    , bIntenseObservationMode(false)
{
}

void UPlayerPerceptionSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    AttentionCapacity = MaxAttentionCapacity;
    CurrentSanity = MaxSanity;
    
    UE_LOG(LogTemp, Warning, TEXT("Player Perception System Initialized"));
}

void UPlayerPerceptionSystem::Deinitialize()
{
    ObservedObjects.Empty();
    PeripheralObjects.Empty();
    Super::Deinitialize();
}

void UPlayerPerceptionSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    UpdatePerception(DeltaTime);
}

void UPlayerPerceptionSystem::SetViewport(const FVector& InCameraLocation, const FRotator& InCameraRotation)
{
    CameraLocation = InCameraLocation;
    CameraRotation = InCameraRotation;
    
    // Calculate camera axes
    CameraForward = CameraRotation.Vector();
    CameraRight = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Y);
    CameraUp = FRotationMatrix(CameraRotation).GetScaledAxis(EAxis::Z);
}

void UPlayerPerceptionSystem::UpdatePerception(float DeltaTime)
{
    PerformRaycastPerception();
    UpdateFocusTarget();
    UpdatePeripheralVision();
    ProcessObservedObjects(DeltaTime);
    UpdateAnomalyDetection();
    UpdateAttentionSystem(DeltaTime);
    UpdateSanitySystem(DeltaTime);
}

float UPlayerPerceptionSystem::GetObservationDuration(AActor* InActor) const
{
    const FObservedObject* ObsPtr = ObservedObjects.Find(InActor);
    return ObsPtr ? ObsPtr->ObservationDuration : 0.0f;
}

void UPlayerPerceptionSystem::RegisterAnomaly(const FVector& AnomalyLocation, float AnomalySeverity)
{
    AnomalyAwareness += AnomalySeverity;
    
    if (AnomalyAwareness >= AnomalyDetectionThreshold)
    {
        bHasNoticedAnomaly = true;
    }
}

void UPlayerPerceptionSystem::ApplyAttentionStress(float StressAmount)
{
    CurrentAttentionLoad = FMath::Min(CurrentAttentionLoad + StressAmount, MaxAttentionCapacity);
}

void UPlayerPerceptionSystem::ReduceSanity(float Amount)
{
    CurrentSanity = FMath::Max(CurrentSanity - Amount, 0.0f);
}

void UPlayerPerceptionSystem::RestoreSanity(float Amount)
{
    CurrentSanity = FMath::Min(CurrentSanity + Amount, MaxSanity);
}

void UPlayerPerceptionSystem::EnterIntenseObservationMode()
{
    bIntenseObservationMode = true;
    MaxPerceptionDistance = MaxPerceptionDistance * 1.5f;
    FieldOfViewAngle = FieldOfViewAngle * 1.3f;
}

void UPlayerPerceptionSystem::ExitIntenseObservationMode()
{
    bIntenseObservationMode = false;
    MaxPerceptionDistance = MaxPerceptionDistance / 1.5f;
    FieldOfViewAngle = FieldOfViewAngle / 1.3f;
}

void UPlayerPerceptionSystem::PerformRaycastPerception()
{
    // Cast rays to detect observed objects
    PeripheralObjects.Empty();
    
    // Center raycast for primary focus
    FHitResult CenterHit;
    FVector RayStart = CameraLocation;
    FVector RayEnd = CameraLocation + CameraForward * MaxPerceptionDistance;
    
    if (GetWorld()->LineTraceSingleByChannel(CenterHit, RayStart, RayEnd, ECC_Pawn))
    {
        if (AActor* HitActor = CenterHit.GetActor())
        {
            PrimaryFocusTarget = HitActor;
            PrimaryFocusIntensity = 1.0f;
        }
    }
    
    // Peripheral vision raycasts
    for (int32 i = 0; i < 8; ++i)
    {
        float Angle = (i / 8.0f) * 360.0f;
        FVector PeripheralDirection = CameraForward.RotateAngleAxis(Angle, CameraUp);
        FVector PeripheralEnd = CameraLocation + PeripheralDirection * MaxPerceptionDistance;
        
        FHitResult PeripheralHit;
        if (GetWorld()->LineTraceSingleByChannel(PeripheralHit, RayStart, PeripheralEnd, ECC_Pawn))
        {
            if (AActor* PeripheralActor = PeripheralHit.GetActor())
            {
                PeripheralObjects.AddUnique(PeripheralActor);
            }
        }
    }
}

void UPlayerPerceptionSystem::UpdateFocusTarget()
{
    // Update focus intensity based on attention
    if (PrimaryFocusTarget)
    {
        PrimaryFocusIntensity = FMath::Lerp(PrimaryFocusIntensity, 1.0f, 0.1f);
        PrimaryFocusDuration += GetWorld()->DeltaTimeSeconds;
    }
    else
    {
        PrimaryFocusIntensity = FMath::Lerp(PrimaryFocusIntensity, 0.0f, 0.1f);
        PrimaryFocusDuration = 0.0f;
    }
    
    // Notify observation system
    if (PrimaryFocusTarget && PrimaryFocusIntensity > 0.1f)
    {
        FObservationData ObsData;
        ObsData.ObservedActor = PrimaryFocusTarget;
        ObsData.ObservationDirection = CameraForward;
        ObsData.ObservationDistance = FVector::Dist(CameraLocation, PrimaryFocusTarget->GetActorLocation());
        ObsData.FocusIntensity = PrimaryFocusIntensity;
        ObsData.ObservationDuration = PrimaryFocusDuration;
        ObsData.bIsDirect = true;
        
        FQuantumObservationSystem::Get().SetPlayerObservation(ObsData);
    }
}

void UPlayerPerceptionSystem::UpdatePeripheralVision()
{
    // Update peripheral object tracking
    for (AActor* PeripheralActor : PeripheralObjects)
    {
        if (!ObservedObjects.Contains(PeripheralActor))
        {
            FObservedObject NewObserved;
            NewObserved.ObjectActor = PeripheralActor;
            NewObserved.ObservationDuration = 0.0f;
            NewObserved.FocusIntensity = 0.3f;  // Lower than primary focus
            NewObserved.LastSeenLocation = PeripheralActor->GetActorLocation();
            NewObserved.bHasBeenSeen = true;
            NewObserved.TimeSinceLastSeen = 0.0f;
            
            ObservedObjects.Add(PeripheralActor, NewObserved);
        }
    }
}

void UPlayerPerceptionSystem::ProcessObservedObjects(float DeltaTime)
{
    for (auto It = ObservedObjects.CreateIterator(); It; ++It)
    {
        AActor* Actor = It.Key();
        FObservedObject& ObsObj = It.Value();
        
        if (Actor)
        {
            ObsObj.ObservationDuration += DeltaTime;
            ObsObj.TimeSinceLastSeen += DeltaTime;
            
            // Remove if not seen for too long
            if (ObsObj.TimeSinceLastSeen > 5.0f)
            {
                It.RemoveCurrent();
            }
        }
        else
        {
            It.RemoveCurrent();
        }
    }
}

void UPlayerPerceptionSystem::UpdateAnomalyDetection()
{
    // Decay anomaly awareness over time
    AnomalyAwareness = FMath::Max(AnomalyAwareness - GetWorld()->DeltaTimeSeconds * 0.1f, 0.0f);
}

void UPlayerPerceptionSystem::UpdateAttentionSystem(float DeltaTime)
{
    // Recover attention capacity over time
    CurrentAttentionLoad = FMath::Max(CurrentAttentionLoad - AttentionRecoveryRate * DeltaTime, 0.0f);
    AttentionCapacity = 1.0f - (CurrentAttentionLoad / MaxAttentionCapacity);
}

void UPlayerPerceptionSystem::UpdateSanitySystem(float DeltaTime)
{
    // Recover sanity slowly over time
    if (bHasNoticedAnomaly)
    {
        CurrentSanity = FMath::Max(CurrentSanity - SanityRecoveryRate * 0.5f * DeltaTime, 0.0f);
    }
    else
    {
        CurrentSanity = FMath::Min(CurrentSanity + SanityRecoveryRate * DeltaTime, MaxSanity);
    }
}

FPerceptionRay UPlayerPerceptionSystem::CreatePerceptionRay(const FVector& Direction)
{
    FPerceptionRay Ray;
    Ray.RayStart = CameraLocation;
    Ray.RayDirection = Direction;
    Ray.RayLength = MaxPerceptionDistance;
    Ray.FocusIntensity = 1.0f;
    Ray.PeripheralType = EPV_DirectFocus;
    return Ray;
}

EPeripheralVision UPlayerPerceptionSystem::CalculatePeripheralType(const FVector& ObjectLocation)
{
    FVector ToObject = (ObjectLocation - CameraLocation).GetSafeNormal();
    float DotProduct = FVector::DotProduct(CameraForward, ToObject);
    
    if (DotProduct > 0.9f)
    {
        return EPV_DirectFocus;
    }
    else if (DotProduct > 0.7f)
    {
        return EPV_Peripheral;
    }
    else if (DotProduct > 0.0f)
    {
        return EPV_Barely;
    }
    else
    {
        return EPV_NotVisible;
    }
}
