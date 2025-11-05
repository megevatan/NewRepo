# Step-by-Step Setup Guide: From C++ to Working UE5 Game

This guide will walk you through the complete process of taking these C++ files and creating a working turn-based strategy game in Unreal Engine 5.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Creating the UE5 Project](#creating-the-ue5-project)
3. [Adding the C++ Files](#adding-the-cpp-files)
4. [Compiling in Visual Studio](#compiling-in-visual-studio)
5. [Creating Blueprints](#creating-blueprints)
6. [Setting Up Input](#setting-up-input)
7. [Testing the Game](#testing-the-game)

---

## Prerequisites

### Required Software

1. **Unreal Engine 5.0+**
   - Download from [Epic Games Launcher](https://www.epicgames.com/store/en-US/download)
   - Install at least version 5.0 or later

2. **Visual Studio 2022**
   - Download [Community Edition](https://visualstudio.microsoft.com/downloads/) (free)
   - During installation, select:
     - "Game development with C++"
     - "Unreal Engine installer" workload (if available)

3. **Required disk space**: ~50GB
   - UE5: ~30GB
   - Visual Studio: ~10GB
   - Project files: ~10GB

### Estimated Time
- First-time setup: 2-3 hours
- If you already have UE5 and VS installed: 30-45 minutes

---

## Creating the UE5 Project

### Option A: Create New Project from Scratch

1. **Open Epic Games Launcher**
   - Go to Unreal Engine → Library
   - Launch Unreal Engine 5.x

2. **Create New Project**
   - Select "Games" → "Blank" template
   - Choose "C++" (NOT Blueprint only!)
   - Project Settings:
     - Starter Content: No
     - Raytracing: Disabled (optional)
     - Target Platform: Desktop
   - Name: `TurnBasedStrategy` (or your preferred name)
   - Click "Create"

3. **Wait for Project Creation**
   - UE5 will:
     - Create project structure
     - Generate Visual Studio files
     - Compile initial code (first compile takes 5-10 minutes)
     - Open the editor

### Option B: Add to Existing C++ Project

If you already have a UE5 C++ project:
1. Just skip to [Adding the C++ Files](#adding-the-cpp-files)

---

## Adding the C++ Files

### Step 1: Close Unreal Editor
- Close the UE5 Editor completely before modifying files
- This prevents file locking issues

### Step 2: Copy Files to Your Project

1. **Locate your project's Source folder**:
   ```
   YourProject/
   └── Source/
       └── YourProjectName/
   ```

2. **Copy ALL these files** from this repository to `Source/YourProjectName/`:
   - `UnitCharacter.h` and `UnitCharacter.cpp`
   - `TBPlayerController.h` and `TBPlayerController.cpp`
   - `TurnHudWidget.h` and `TurnHudWidget.cpp`
   - `TurnStatsComponent.h` and `TurnStatsComponent.cpp`
   - `AGridTile.h` and `AGridTile.cpp`
   - `AGridManager.h` and `AGridManager.cpp`

3. **Important: Fix the API macro**
   - Open each `.h` file
   - Find `DENEME_API` (this is the old project name)
   - Replace with `YOURPROJECTNAME_API`
   - Example: If your project is "TurnBasedStrategy", replace with `TURNBASEDSTRATEGY_API`
   - This must be done in ALL header files!

### Step 3: Verify File Structure

Your structure should look like:
```
TurnBasedStrategy/
├── Source/
│   └── TurnBasedStrategy/
│       ├── TurnBasedStrategy.Build.cs
│       ├── TurnBasedStrategy.cpp
│       ├── TurnBasedStrategy.h
│       ├── UnitCharacter.h
│       ├── UnitCharacter.cpp
│       ├── TBPlayerController.h
│       ├── TBPlayerController.cpp
│       ├── TurnHudWidget.h
│       ├── TurnHudWidget.cpp
│       ├── TurnStatsComponent.h
│       ├── TurnStatsComponent.cpp
│       ├── AGridTile.h
│       ├── AGridTile.cpp
│       ├── AGridManager.h
│       └── AGridManager.cpp
├── Content/
├── Config/
└── TurnBasedStrategy.uproject
```

---

## Compiling in Visual Studio

### Step 1: Generate Visual Studio Project Files

1. **Right-click** your `.uproject` file in Windows Explorer
2. Select **"Generate Visual Studio project files"**
3. Wait for generation to complete (30-60 seconds)
4. You should now see a `.sln` file

### Step 2: Open in Visual Studio

1. **Double-click** the `.sln` file
2. Visual Studio 2022 will open
3. If prompted about retargeting, click "OK"

### Step 3: Configure Build Settings

1. At the top toolbar, set:
   - Configuration: **`Development Editor`**
   - Platform: **`Win64`**

2. In Solution Explorer (right side):
   - Find your project (under "Games" folder)
   - Right-click → "Set as Startup Project" (makes it bold)

### Step 4: Build the Project

1. **Menu: Build → Build Solution** (or press `Ctrl+Shift+B`)

2. **Watch the Output window** (bottom of VS):
   - You should see compilation progress
   - First build takes 5-15 minutes
   - Look for "Build succeeded" message

3. **If you see errors**:
   - Check that you replaced `DENEME_API` with your project's API macro
   - Ensure all files are in the correct directory
   - Common error: "cannot open include file" means you're missing a file

### Step 5: Verify Compilation

Look for these messages in Output:
```
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

If successful, you're ready for UE5!

---

## Creating Blueprints

### Step 1: Open Your Project in UE5

1. If not already open, launch your project from Epic Games Launcher
2. Editor will take 2-3 minutes to load
3. The editor reloads your newly compiled C++ classes

### Step 2: Create Grid Tile Blueprint

1. **Content Browser** → Right-click → **Blueprint Class**
2. Search for "AGridTile" → Select it as parent
3. Name it: `BP_GridTile`
4. **Open the Blueprint**:
   - Add Component → **Static Mesh**
   - In Details panel:
     - Static Mesh → Select "Cube" (or any mesh you want)
     - Scale → Set to (1.0, 1.0, 0.1) for a flat tile
   - Add Component → **Box Collision** (for mouse detection)
     - Scale to match mesh
   - Save

### Step 3: Create Grid Manager Blueprint

1. **Content Browser** → Right-click → **Blueprint Class**
2. Search for "AGridManager" → Select it
3. Name it: `BP_GridManager`
4. **Open the Blueprint**:
   - In Class Defaults (right panel):
     - Grid Width: `10`
     - Grid Height: `10`
     - Tile Size: `100.0`
     - Tile Class → Select `BP_GridTile`
   - Save

### Step 4: Create Unit Character Blueprint

1. **Content Browser** → Right-click → **Blueprint Class**
2. Search for "UnitCharacter" → Select it
3. Name it: `BP_UnitCharacter`
4. **Open the Blueprint**:
   - Add Component → **Skeletal Mesh** (or Static Mesh for testing)
     - Static Mesh → Select "Sphere" or "Cylinder"
     - Scale → Set to (0.8, 0.8, 1.5)
   - In Class Defaults:
     - Max HP: `100`
     - Configure abilities as desired
   - Save

### Step 5: Create Turn HUD Widget

1. **Content Browser** → Right-click → **User Interface** → **Widget Blueprint**
2. Name it: `WBP_TurnHud`

3. **Switch to Graph mode** (button in top-right):
   - File → Reparent Blueprint
   - Search for "TurnHudWidget"
   - Select it → Reparent

4. **Switch back to Designer mode**:
   - Drag widgets from the Palette to create UI
   - **CRITICAL**: Names must match C++ exactly!

   Required widgets (drag from Palette → Common):
   
   | Widget Type | Name (must be exact!) | Purpose |
   |------------|----------------------|---------|
   | TextBlock  | `TurnTimerText`      | Show turn timer |
   | TextBlock  | `TurnStateText`      | Show game state |
   | TextBlock  | `UnitHPText`         | Show unit HP |
   | TextBlock  | `UnitMPText`         | Show movement points |
   | TextBlock  | `UnitAPText`         | Show action points |
   | Button     | `EndTurnButton`      | End turn |
   | Button     | `ConfirmButton`      | Confirm move |
   | Button     | `CastMagicArrowButton` | Cast ability |
   | Button     | `CastBoulderButton`  | Cast ability |

5. **For each widget**:
   - Select it in Hierarchy
   - In Details panel → Check "Is Variable"
   - Set the exact name from the table above

6. **Style the UI** (optional):
   - Position widgets as desired
   - Set text size, colors, fonts
   - Add button text labels

7. **Compile and Save**

### Step 6: Create Player Controller Blueprint

1. **Content Browser** → Right-click → **Blueprint Class**
2. Search for "TBPlayerController" → Select it
3. Name it: `BP_TBPlayerController`
4. **Open the Blueprint**:
   - In Class Defaults:
     - Turn Hud Class → Select `WBP_TurnHud`
   - Save

### Step 7: Create Game Mode

1. **Content Browser** → Right-click → **Blueprint Class**
2. Select "Game Mode Base"
3. Name it: `BP_TurnBasedGameMode`
4. **Open the Blueprint**:
   - In Class Defaults:
     - Player Controller Class → Select `BP_TBPlayerController`
     - Default Pawn Class → Set to "None" (we don't use a pawn)
   - Save

---

## Setting Up Input

### Step 1: Configure Input Mappings

1. **Menu: Edit → Project Settings**
2. **Input → Bindings**
3. **Action Mappings** → Click **+** to add:

   | Name | Key |
   |------|-----|
   | `LeftClick` | Left Mouse Button |
   | `RightClick` | Right Mouse Button |
   | `Confirm` | Space Bar |
   | `CancelPreview` | Escape |

4. **Close Project Settings**

---

## Testing the Game

### Step 1: Set Up the Level

1. **In the Level Editor**:
   - Delete default objects (floor, lights if you want)
   - Keep the lighting for visibility

2. **Drag `BP_GridManager` into the level**:
   - Place it at (0, 0, 0)
   - In Details panel, click "Generate Grid" (if you added a button in Blueprint)
   - OR: Add this in BP_GridManager's BeginPlay event to auto-generate

3. **Add some units**:
   - Drag `BP_UnitCharacter` into level (2-3 times)
   - Position them on different tiles
   - For each unit:
     - In Details → Find "Current Tile"
     - You may need to set this in BeginPlay or manually

4. **Set World Settings**:
   - Menu: Settings → World Settings
   - Game Mode Override → Select `BP_TurnBasedGameMode`

### Step 2: Play Test

1. **Click Play** (Alt+P)

2. **Expected behavior**:
   - Mouse cursor visible
   - UI appears at top
   - Click a unit to select it
   - Right-click a tile to preview movement
   - Press Space to confirm movement
   - UI shows updated stats

3. **Troubleshooting**:
   - No UI? Check Widget is set in PlayerController
   - Can't select units? Check collision on GridTile and UnitCharacter
   - No grid? Check BP_GridManager generated tiles in BeginPlay
   - Input not working? Verify Action Mappings

### Step 3: Debug If Needed

1. **Open Output Log**:
   - Window → Developer Tools → Output Log
   - Look for errors

2. **Blueprint Debugging**:
   - Add Print String nodes to test events
   - Check if functions are being called

3. **C++ Debugging**:
   - Close UE5 Editor
   - In Visual Studio: Debug → Start Debugging (F5)
   - UE5 will launch attached to debugger
   - Set breakpoints in C++ code

---

## Next Steps

### Enhance Your Game

1. **Add AI opponents**:
   - Create AI Controller class
   - Implement turn logic

2. **Visual improvements**:
   - Add mesh assets for units and tiles
   - Create materials and textures
   - Add particle effects for abilities

3. **More abilities**:
   - Create ability data structures
   - Add AOE abilities, buffs, debuffs

4. **Game flow**:
   - Add win/lose conditions
   - Implement turn timer
   - Add menu screens

### Learning Resources

- [UE5 C++ Tutorials](https://docs.unrealengine.com/5.0/en-US/programming-with-cplusplus-in-unreal-engine/)
- [UMG UI Designer](https://docs.unrealengine.com/5.0/en-US/umg-ui-designer-for-unreal-engine/)
- [Blueprint Visual Scripting](https://docs.unrealengine.com/5.0/en-US/blueprints-visual-scripting-in-unreal-engine/)

---

## Common Issues and Solutions

### "Cannot find module"
- **Cause**: Project name mismatch
- **Solution**: Ensure API macro matches your project name

### "Unresolved external symbol"
- **Cause**: Missing function implementations
- **Solution**: Check all `.cpp` files are in project and compiled

### BindWidget errors
- **Cause**: Widget names don't match C++
- **Solution**: Names must be EXACT, case-sensitive

### Path not found in gameplay
- **Cause**: GridManager not set up correctly
- **Solution**: Ensure tiles have collision, check pathfinding logic

### UI not updating
- **Cause**: Delegates not bound
- **Solution**: Check SetTargetUnit is called when selecting unit

---

## Success Checklist

- [ ] Visual Studio compiles without errors
- [ ] UE5 shows your C++ classes in Blueprint parent selection
- [ ] BP_GridManager spawns grid tiles
- [ ] Can click to select units
- [ ] Can right-click to preview movement
- [ ] Can press Space to confirm movement
- [ ] UI shows unit stats
- [ ] Can cast abilities with buttons
- [ ] HP decreases when unit takes damage

If all checked, congratulations! You have a working foundation for a turn-based strategy game!

---

## Getting Help

If you're stuck:
1. Check Output Log for errors
2. Verify all steps were followed exactly
3. Compare your Blueprint settings to this guide
4. Check UE5 documentation for specific features
5. Search UE5 AnswerHub or forums for specific error messages

Remember: Game development is iterative. Don't expect perfection on first try!
