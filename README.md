# Turn-Based Strategy Game - UE5 C++ Components

This repository contains C++ classes for a turn-based strategy game built with Unreal Engine 5 (UE5).

## What's Included

This repository contains three main C++ classes:
- **UnitCharacter** - Character/unit system with HP, abilities (Magic Arrow, Boulder), movement preview/confirmation
- **TBPlayerController** - Player controller for mouse input, unit selection, and ability casting
- **TurnHudWidget** - UMG widget for displaying unit stats and action buttons

## Understanding C++ and Blueprints in UE5

### Do You Need Both?

**YES** - Unreal Engine 5 development typically involves both C++ and Blueprints working together:

1. **C++ Code (These Files)** - Provides the core logic, performance-critical systems, and base classes
2. **Blueprints (Created in UE5)** - Extends C++ classes, configures properties, creates visual logic, and designs UI

### The Workflow

The typical workflow is:

```
C++ Code → Compile in Visual Studio → Use in UE5 → Create Blueprints → Design Gameplay
```

**You cannot skip compilation** - The C++ code must be compiled before it can be used in UE5.

## Setup Instructions

### Prerequisites

- **Unreal Engine 5** (5.0 or later)
- **Visual Studio 2022** (with C++ game development workload)
- **A UE5 C++ Project** (these files need to be part of a UE5 project)

### Step 1: Integrating Into Your UE5 Project

These C++ files are **not a standalone project**. They need to be integrated into a UE5 C++ project:

1. Create a new UE5 C++ project (or use an existing one)
2. Copy these `.h` and `.cpp` files to your project's `Source/[YourProjectName]/` directory
3. Your project structure should look like:
   ```
   YourProject/
   ├── Source/
   │   └── YourProjectName/
   │       ├── UnitCharacter.h
   │       ├── UnitCharacter.cpp
   │       ├── TBPlayerController.h
   │       ├── TBPlayerController.cpp
   │       ├── TurnHudWidget.h
   │       └── TurnHudWidget.cpp
   ├── Content/
   ├── YourProject.uproject
   └── ...
   ```

### Step 2: Add Missing Dependencies

These files reference several classes that are not included in this repository. You'll need to create them:

#### Required Classes:
- **UTurnStatsComponent** - Component that tracks MovementPoints, ActionPoints, and manages turn resources
- **AGridTile** - Actor representing a grid tile with X, Y coordinates and Occupant pointer
- **AGridManager** - Actor that manages the grid and pathfinding (FindPath method)

#### Minimal Implementation Example for TurnStatsComponent:

```cpp
// TurnStatsComponent.h
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnStatsComponent.generated.h"

UCLASS()
class YOURPROJECT_API UTurnStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
    int32 MovementPoints = 10;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
    int32 MaxMovementPoints = 10;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
    int32 ActionPoints = 5;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
    int32 MaxActionPoints = 5;
    
    bool SpendMovement(int32 Cost);
    bool SpendAction(int32 Cost);
    void ResetForNewTurn();
};
```

### Step 3: Compile in Visual Studio

1. **Generate Visual Studio Project Files**:
   - Right-click your `.uproject` file
   - Select "Generate Visual Studio project files"

2. **Open in Visual Studio**:
   - Double-click the generated `.sln` file to open in Visual Studio 2022

3. **Build the Project**:
   - Select `Development Editor` configuration
   - Build → Build Solution (or press `Ctrl+Shift+B`)
   - Wait for compilation to complete (may take several minutes first time)

4. **Fix Any Compilation Errors**:
   - Add missing includes
   - Implement missing dependency classes
   - Ensure all referenced classes exist

### Step 4: Create Blueprints in UE5

After successful compilation, open your project in UE5:

#### 4.1 Create Blueprint Classes

1. **Unit Character Blueprint**:
   - Content Browser → Right-click → Blueprint Class
   - Search for "UnitCharacter" as parent class
   - Name it `BP_UnitCharacter`
   - Add a Skeletal Mesh component for visuals
   - Configure default HP, abilities, and stats

2. **Player Controller Blueprint**:
   - Create Blueprint derived from `TBPlayerController`
   - Name it `BP_TBPlayerController`
   - Set as default Player Controller in Game Mode

3. **HUD Widget**:
   - Create a Widget Blueprint (User Interface → Widget Blueprint)
   - Open the widget designer
   - Add UI elements with exact names matching C++ (BindWidget):
     - `TurnTimerText` (TextBlock)
     - `TurnStateText` (TextBlock)
     - `UnitHPText` (TextBlock)
     - `UnitMPText` (TextBlock)
     - `UnitAPText` (TextBlock)
     - `EndTurnButton` (Button)
     - `ConfirmButton` (Button)
     - `CastMagicArrowButton` (Button)
     - `CastBoulderButton` (Button)
   - Set the parent class to `TurnHudWidget` in the Graph tab

#### 4.2 Configure Input Mappings

In Project Settings → Input:
- Add Action Mapping: `LeftClick` → Left Mouse Button
- Add Action Mapping: `RightClick` → Right Mouse Button
- Add Action Mapping: `Confirm` → Space Bar (or any key)
- Add Action Mapping: `CancelPreview` → Escape

#### 4.3 Set Up Game Mode

1. Create a Blueprint Game Mode
2. Set Player Controller Class to `BP_TBPlayerController`
3. Set Default Pawn Class to None (or your custom pawn if needed)

#### 4.4 Create Grid System

You'll need to implement or create Blueprints for:
- GridTile actor with collision for mouse detection
- GridManager actor with pathfinding logic
- Spawn a grid of tiles in your level

## Can This Be Automated?

**Partially, but not entirely:**

### What Can Be Automated:
- ✅ Build automation through command-line builds
- ✅ CI/CD for compilation testing
- ✅ Blueprint generation via Python/Editor Utility Widgets

### What Requires Manual Work:
- ❌ Visual Studio compilation (first time setup)
- ❌ Blueprint asset creation and configuration in UE5 Editor
- ❌ UMG widget layout and design
- ❌ Level design and grid placement
- ❌ Material and visual asset setup

## Typical Development Cycle

```
1. Write/Modify C++ code
2. Save files
3. Compile in Visual Studio (or use UE5's "Compile" button)
4. If errors: Fix → Repeat step 3
5. If successful: Unreal Editor reloads new code
6. Update/Create Blueprints that use the C++ classes
7. Test in Editor
8. Iterate
```

## Key Points About C++ and Blueprints

- **C++ is compiled code** - Must be built before UE5 can use it
- **Blueprints are visual scripts** - Created in UE5 Editor, extend C++ classes
- **They work together** - C++ provides foundation, Blueprints add flexibility
- **You need both** - Pure Blueprint projects exist, but this code requires C++ compilation
- **Hot reloading** - Minor C++ changes can be compiled while Editor is open (Ctrl+Alt+F11)

## Project Structure Notes

This repository contains **only the C++ source files**. A complete UE5 project would include:
- `.uproject` file
- `Config/` directory (project settings)
- `Content/` directory (assets, blueprints, levels)
- `Source/` directory (C++ code - where these files belong)
- `Binaries/` and `Intermediate/` (generated by build)

## Troubleshooting

### "Cannot find module" errors
- Ensure `.uproject` file is configured correctly
- Check that module name in code matches your project name
- Regenerate Visual Studio project files

### "Unresolved external symbol" errors
- Missing dependency class implementations
- Check that all referenced classes are defined and linked

### "BindWidget" not working
- UI element names in UMG must exactly match C++ variable names
- Widget element must be marked as `Is Variable` in UMG designer

### UE5 doesn't show C++ classes
- Ensure compilation succeeded without errors
- Restart UE5 Editor
- Check that class has `UCLASS()` macro and proper API macro

## Additional Resources

- [Unreal Engine C++ Documentation](https://docs.unrealengine.com/5.0/en-US/unreal-engine-cpp-api/)
- [Blueprint and C++ Guide](https://docs.unrealengine.com/5.0/en-US/blueprints-visual-scripting-in-unreal-engine/)
- [UMG UI Designer Documentation](https://docs.unrealengine.com/5.0/en-US/umg-ui-designer-for-unreal-engine/)

## Summary

**To answer the original question**: No, these files cannot work without both Visual Studio compilation AND Blueprint setup in UE5. Both steps are necessary:

1. **Compile C++ in Visual Studio** - Builds the core logic
2. **Create Blueprints in UE5** - Configures and extends the C++ classes for your specific game

This is the standard UE5 C++ workflow and cannot be bypassed.
