# OBSERVATION: PARADOX - Technical Architecture Guide

## System Overview

This document outlines the technical architecture and implementation details for the Observation Paradox game engine.

### High-Level Architecture

```
┌─────────────────────────────────────────────────────┐
│         OBSERVATION PARADOX ENGINE (UE5)           │
├─────────────────────────────────────────────────────┤
│                                                     │
│  ┌───────────────────────────────────────────────┐ │
│  │   QUANTUM OBSERVATION SYSTEM (Core Mechanic) │ │
│  ├───────────────────────────────────────────────┤ │
│  │ • Tracks player observation direction        │ │
│  │ • Manages quantum state superposition        │ │
│  │ • Collapses reality on observation          │ │
│  │ • Generates uncertainty variations          │ │
│  └───────────────────────────────────────────────┘ │
│           ↓                    ↓                    │
│  ┌──────────────────┐  ┌──────────────────┐       │
│  │  OBSERVABLE     │  │  REALITY         │       │
│  │  ACTORS         │  │  MANAGER         │       │
│  │                │  │                  │       │
│  │ • NPCs         │  │ • Zone Types     │       │
│  │ • Buildings    │  │ • Corruption     │       │
│  │ • Street       │  │ • Time Effects   │       │
│  │ • Objects      │  │ • Transitions    │       │
│  └──────────────────┘  └──────────────────┘       │
│           ↓                    ↓                    │
│  ┌──────────────────┐  ┌──────────────────┐       │
│  │  PROCEDURAL     │  │  PLAYER          │       │
│  │  NYC            │  │  PERCEPTION      │       │
│  │                │  │                  │       │
│  │ • Streets      │  │ • Observation    │       │
│  │ • Buildings    │  │ • Attention      │       │
│  │ • Mutations    │  │ • Sanity         │       │
│  │ • Locking      │  │ • Anomaly Detect │       │
│  └──────────────────┘  └──────────────────┘       │
│           ↓                    ↓                    │
│  ┌──────────────────┐  ┌──────────────────┐       │
│  │  QUANTUM        │  │  GLITCH          │       │
│  │  AUDIO          │  │  EFFECTS         │       │
│  │                │  │                  │       │
│  │ • Real/Uncertain│ │ • Geometry Warp  │       │
│  │ • Psychological│ │ • Chromatic Shift│       │
│  │ • Distorted    │  │ • Screen Tearing │       │
│  │ • Impossible   │  │ • Glitching      │       │
│  └──────────────────┘  └──────────────────┘       │
│           ↓                                        │
│  ┌──────────────────────────────────────────────┐ │
│  │    GAME STATE (Story progression)            │ │
│  │ • Acts (Awakening → Paradox Tower)          │ │
│  │ • Global corruption level                   │ │
│  │ • Paradox loop counter                      │ │
│  └──────────────────────────────────────────────┘ │
│                                                     │
└─────────────────────────────────────────────────────┘
```

## Core Loop

### Per-Frame Update Sequence

```
1. Player Input & Camera Update
   ↓
2. Player Perception System
   - Raycast to determine primary focus target
   - Track peripheral vision objects
   - Calculate observation intensity (0.0-1.0)
   ↓
3. Quantum Observation System
   - Update observation state for focused actor
   - Collapse reality if intensity > threshold
   - Trigger uncertainty for unobserved actors
   ↓
4. Observable Actors Update
   - Apply quantum state transitions
   - Execute glitch effects
   - Update based on observation state
   ↓
5. Procedural NYC System
   - Check locked geometry
   - Mutate unobserved districts
   - Stream in/out regions
   ↓
6. Reality Manager
   - Update zone effects
   - Spread corruption
   - Apply environmental changes
   ↓
7. Audio System
   - Update ambient layers based on corruption
   - Play quantum audio events
   - Adjust for observation state
   ↓
8. Glitch Effect System
   - Update active glitch layers
   - Apply visual distortions
   - Update material parameters
   ↓
9. Game State
   - Check act transitions
   - Update story progression
   - Check ending conditions
   ↓
10. Render
```

## Memory Architecture

### Observation State Tracking

```cpp
PerObject:
- Observation state (8 bytes)
- Current quantum state (100+ bytes)
- Possible states array (8+ bytes per state)
- Last observation time (4 bytes)
- Glitch parameters (20+ bytes)

Total per observable: ~500 bytes minimum
With 10,000 observables: ~5 MB
```

### Procedural Content

```cpp
PerDistrict:
- Streets: 20-50 × ~100 bytes = ~5 KB
- Buildings: 40-50 × ~200 bytes = ~10 KB
- Metadata: ~1 KB

Total per district: ~16 KB
With 100 loaded districts: ~1.6 MB
```

### Audio System

```cpp
Ambient Layers: 8-16 × ~500 bytes = ~8 KB
Active Audio Events: variable (capped at 64) = ~32 KB
Whisper Queue: variable (capped at 32) = ~16 KB
```

## Threading Considerations

### Thread-Safe Systems
- **Quantum Observation**: Main thread only (game state dependent)
- **Procedural Generation**: Can be async (careful with locking)
- **Audio Updates**: Can be on audio thread
- **Glitch Effects**: GPU-side (shader compute)

## Network Architecture (Future Multiplayer)

### Observation Conflict Resolution
```
Player A observes: Building X at position (0,0,0)
Player B observes: Building X at position (100,0,0)

Conflict Resolution:
1. Server authoritative observation state
2. Clients send observation data
3. Server determines which observation wins
4. Or: Blend observations into combined reality
5. Or: Create separate branches for each player
```

## Debugging & Profiling

### Built-in Debug Visualization

```cpp
// Enable with console command:
// Shows observation state for all actors
ShowObservationStates

// Shows reality zones with color coding
ShowRealityZones

// Shows quantum states as wireframe overlay
ShowQuantumStates

// Performance metrics
ShowPerformanceMetrics
```

## Load Times & Streaming

### Initial Load
- ~30 seconds (UE5 startup)
- Procedural NYC generation: ~5-10 seconds
- Populate with NPCs: ~2-3 seconds

### Runtime Streaming
- Stream in regions on demand
- Unload far regions (> 10km away)
- Smooth transitions between loaded areas

## Physics Considerations

### Observation-Based Physics

```cpp
When Observed:
- Standard Unreal physics
- Gravity = 9.8 m/s²
- Collisions enabled
- Movement predictable

When Unobserved:
- Relaxed physics constraints
- Actors can teleport
- No collision necessary
- Movement probabilistic
```

## Scalability

### Quality Settings

```
Ultra (8K):
- 8K resolution
- Max observable actors: 10,000
- Max glitch layers: 20
- Full audio detail

High (4K):
- 4K resolution  
- Max observable actors: 5,000
- Max glitch layers: 15
- Reduced whispers

Medium (1440p):
- 1440p resolution
- Max observable actors: 2,000
- Max glitch layers: 10
- Basic audio

Low (1080p):
- 1080p resolution
- Max observable actors: 1,000
- Max glitch layers: 5
- Minimal audio effects
```

---

## Integration with Unreal Engine 5 Systems

### Character Movement
- `ACharacter` base for NPCs
- `UCharacterMovementComponent` for walking/running
- Observation affects movement speed and animation

### Physics
- `UPhysicsEngine` for observed actors
- Probability-based physics for unobserved
- Fracture effects use physics constraints

### Audio
- `UAudioComponent` for spatial audio
- Real-time DSP for distortion/reverb
- 3D audio positioning

### Materials
- Dynamic material instances for glitches
- Custom shaders for reality distortion
- Post-process effects for screen effects

---

For implementation details, see individual system documentation.
