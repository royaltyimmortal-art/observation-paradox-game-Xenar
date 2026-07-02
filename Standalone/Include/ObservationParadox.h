#ifndef OBSERVATION_PARADOX_H
#define OBSERVATION_PARADOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <random>
#include <cmath>
#include <iostream>
#include <array>
#include <deque>

// ============================================================
// OBSERVATION PARADOX - STANDALONE C++ ENGINE
// Pure C++17, No External Engine Dependencies
// ============================================================

namespace ObservationParadox {

// ============================================================
// ENUMS & CONSTANTS
// ============================================================

enum class EObservationState : uint8_t {
    Unobserved,    // Reality is fluid, undefined
    Collapsing,    // Transitioning to observed state
    Observed,      // Reality is locked
    Uncertainty,   // In quantum flux, possible states exist
    Fractured,     // Reality is breaking down
    Null           // Non-euclidean, liminal space
};

enum class ERealityZone : uint8_t {
    Stable,        // Normal NYC physics
    Fracture,      // Reality glitches, shifting geometry
    Null,          // Backrooms-style infinite spaces
    Threshold,     // Transitional areas between zones
    Mirrored       // Reflected/duplicate realities
};

enum class ENPCState : uint8_t {
    Normal,        // Regular behavior
    Aware,         // Knows it's being observed
    Frozen,        // Completely paralyzed by observation
    Glitching,     // Reality distortion around NPC
    Alternative,   // Different personality/appearance
    Fractured,     // Multiple versions overlapping
    Unraveling     // Breaking down reality around self
};

enum class EGameAct : uint8_t {
    Awakening,     // Act 1: Player wakes in NYC apartment
    Collapse,      // Act 2: Reality becomes unstable
    Fracture,      // Act 3: Sub-realities and backrooms
    Loop,          // Act 4: Events repeat with variations
    ParadoxTower,  // Final act: Hidden structure
    EndingSequence // Final truth sequence
};

enum class EPlayerUnderstanding : uint8_t {
    Confused,           // "What's happening?"
    NoticesAnomalies,   // "Something is wrong"
    RealityShifts,      // "The world changes when I'm not looking"
    ObservationEffect,  // "My observation creates reality"
    ParadoxAwareness,   // "Reality has no base state"
    MetaAwareness,      // "I am being observed"
    TruthRevealed       // "There is no escape"
};

// ============================================================
// DATA STRUCTURES
// ============================================================

struct FVector {
    float X, Y, Z;
    
    FVector() : X(0), Y(0), Z(0) {}
    FVector(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}
    
    FVector operator+(const FVector& Other) const {
        return FVector(X + Other.X, Y + Other.Y, Z + Other.Z);
    }
    
    FVector operator-(const FVector& Other) const {
        return FVector(X - Other.X, Y - Other.Y, Z - Other.Z);
    }
    
    FVector operator*(float Scale) const {
        return FVector(X * Scale, Y * Scale, Z * Scale);
    }
    
    float Dot(const FVector& Other) const {
        return X * Other.X + Y * Other.Y + Z * Other.Z;
    }
    
    float Length() const {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }
    
    FVector GetNormalized() const {
        float Len = Length();
        if (Len > 0.0001f) return *this * (1.0f / Len);
        return FVector(0, 0, 0);
    }
    
    static float Distance(const FVector& A, const FVector& B) {
        return (B - A).Length();
    }
};

struct FQuaternion {
    float X, Y, Z, W;
    
    FQuaternion() : X(0), Y(0), Z(0), W(1) {}
    FQuaternion(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
};

struct FRotator {
    float Pitch, Yaw, Roll;
    
    FRotator() : Pitch(0), Yaw(0), Roll(0) {}
    FRotator(float InPitch, float InYaw, float InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}
};

struct FQuantumState {
    FVector Location;
    FRotator Rotation;
    FVector Scale;
    bool bVisible;
    bool bPhysicsEnabled;
    uint8_t VariationID;
    float Probability;
    std::string Description;
    uint8_t StateHash;
};

struct FObservationData {
    void* ObservedActor;  // Pointer to actor
    FVector ObservationDirection;
    float ObservationDistance;
    float FocusIntensity;  // 0.0 to 1.0
    float ObservationDuration;
    bool bIsDirect;
};

struct FRealityZoneData {
    FVector ZoneCenter;
    float ZoneRadius;
    ERealityZone ZoneType;
    float TimeMultiplier;
    float CorruptionLevel;
    bool bIsActive;
    float LastUpdateTime;
};

struct FBuildingData {
    FVector BaseLocation;
    FVector CurrentLocation;
    FVector Scale;
    FRotator Rotation;
    int BuildingType;  // 0: Residential, 1: Commercial, 2: Office, 3: Industrial
    int Floors;
    std::string ModelID;
    bool bIsGenerated;
    bool bIsObserved;
    uint32_t LocationHash;
};

struct FNPCMemory {
    std::string Description;
    float TimeSinceEvent;
    bool bIsImpossible;
    float Probability;
};

struct FDialogueNode {
    std::string DialogueText;
    ENPCState TriggerState;
    float ObservationThreshold;
    bool bIsMetaCommentary;
    std::string AudioID;
};

// ============================================================
// FORWARD DECLARATIONS
// ============================================================

class FQuantumObservationSystem;
class AObservableActor;
class AQuantumNPC;
class URealityManager;
class UProceduralNYC;
class UPlayerPerceptionSystem;
class UQuantumAudioSystem;
class FGlitchEffectSystem;
class AGameState;

// ============================================================
// UTILITY FUNCTIONS
// ============================================================

namespace Math {
    inline float FRand() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }
    
    inline int RandRange(int Min, int Max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(Min, Max);
        return dis(gen);
    }
    
    inline float FRandRange(float Min, float Max) {
        return Min + FRand() * (Max - Min);
    }
    
    inline float Lerp(float A, float B, float Alpha) {
        return A + (B - A) * Alpha;
    }
    
    inline float Clamp(float Value, float Min, float Max) {
        if (Value < Min) return Min;
        if (Value > Max) return Max;
        return Value;
    }
    
    inline bool RandBool() {
        return FRand() > 0.5f;
    }
}

}  // namespace ObservationParadox

#endif // OBSERVATION_PARADOX_H
