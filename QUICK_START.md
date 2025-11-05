# Quick Start Guide (For Experienced UE5 Developers)

**TL;DR**: This repo contains C++ source files for a turn-based strategy game. You need to compile them in VS and create Blueprints in UE5. Both steps are required.

## Immediate Setup (5 Minutes)

### 1. Copy Files to Your UE5 C++ Project
```bash
cp *.h *.cpp YourProject/Source/YourProjectName/
```

### 2. Fix API Macro
Replace `DENEME_API` with `YOURPROJECTNAME_API` in all `.h` files.

### 3. Regenerate & Compile
```bash
# Right-click .uproject → "Generate Visual Studio project files"
# Open .sln in VS2022
# Build → Build Solution (Development Editor config)
```

## Dependencies Included

All dependency classes are now included:
- `UTurnStatsComponent` - Tracks MP/AP
- `AGridTile` - Grid tile with coordinates and occupant
- `AGridManager` - Grid management and A* pathfinding

## Blueprint Setup (10 Minutes)

### Required Blueprints

1. **BP_GridTile** (parent: AGridTile)
   - Add StaticMesh + BoxCollision

2. **BP_GridManager** (parent: AGridManager)
   - Set TileClass = BP_GridTile
   - Set GridWidth/Height
   - Call GenerateGrid() in BeginPlay

3. **BP_UnitCharacter** (parent: UnitCharacter)
   - Add SkeletalMesh or StaticMesh

4. **WBP_TurnHud** (Widget Blueprint, reparent to TurnHudWidget)
   - Add TextBlocks: `TurnTimerText`, `TurnStateText`, `UnitHPText`, `UnitMPText`, `UnitAPText`
   - Add Buttons: `EndTurnButton`, `ConfirmButton`, `CastMagicArrowButton`, `CastBoulderButton`
   - Names must match exactly (BindWidget)

5. **BP_TBPlayerController** (parent: TBPlayerController)
   - Set TurnHudClass = WBP_TurnHud

6. **BP_GameMode** (parent: GameModeBase)
   - Set PlayerControllerClass = BP_TBPlayerController
   - Set DefaultPawnClass = None

### Input Mappings

Add Action Mappings in Project Settings → Input:
- `LeftClick` → Left Mouse Button
- `RightClick` → Right Mouse Button
- `Confirm` → Space
- `CancelPreview` → Escape

## Quick Test

1. Place BP_GridManager in level
2. Place 2x BP_UnitCharacter on grid
3. Set World Settings → GameMode = BP_GameMode
4. Play
5. Click unit, right-click tile, press Space to move

## Architecture Overview

```
Player Input (Mouse/Keyboard)
    ↓
TBPlayerController (handles clicks, gets tile/unit under cursor)
    ↓
UnitCharacter (movement preview, ability casting)
    ↓
TurnStatsComponent (tracks MP/AP)
    ↓
GridManager (pathfinding via A*)
    ↓
GridTile (occupancy tracking)
```

## Key Features

- **Movement System**: Preview → Confirm workflow
- **Abilities**: Magic Arrow (ranged) and Boulder (melee)
- **Turn Stats**: MP for movement, AP for abilities
- **Grid Pathfinding**: A* implementation with occupancy checking
- **UI Binding**: Automatic stat updates via delegates

## Extension Points

- Add more abilities in UnitCharacter
- Create AI controller for enemy units
- Implement turn management system
- Add combat animations and VFX
- Create ability selection UI

## Common Gotchas

- **API Macro**: Must match your project name
- **BindWidget**: UI element names must be exact
- **Collision**: Tiles need collision for mouse picking
- **Grid Generation**: Call GenerateGrid() or add tiles manually
- **CurrentTile**: Units need CurrentTile set to starting position

## File Overview

| File | Purpose |
|------|---------|
| UnitCharacter | Actor with HP, abilities, movement |
| TBPlayerController | Handles input, unit selection |
| TurnHudWidget | UMG widget for stats display |
| TurnStatsComponent | MP/AP tracking component |
| AGridTile | Individual tile with coordinates |
| AGridManager | Grid spawning and pathfinding |

## What You Still Need to Create

- **Visuals**: Meshes, materials, animations
- **Game Logic**: Turn manager, win/lose conditions
- **UI Polish**: Menu screens, ability tooltips
- **Content**: More units, abilities, levels

## Compilation Requirements

**Cannot be skipped:**
- Visual Studio 2022 with C++ game dev workload
- Unreal Engine 5.0+
- C++ compilation before Blueprint use

**This is standard UE5 workflow** - C++ provides base classes, Blueprints extend and configure them.

---

For detailed instructions, see [SETUP_GUIDE.md](SETUP_GUIDE.md)
