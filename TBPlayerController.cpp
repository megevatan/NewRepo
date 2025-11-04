#include "TBPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UnitCharacter.h"
#include "AGridTile.h"
#include "GridManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TurnHudWidget.h"

ATBPlayerController::ATBPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
    CachedGridManager = nullptr;
}

void ATBPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // Cache GridManager pointer to avoid iterating actors on each right-click
    for (TActorIterator<AGridManager> It(GetWorld()); It; ++It)
    {
        if (IsValid(*It))
        {
            CachedGridManager = *It;
            break;
        }
    }
    
    // Safely create and add the widget
    if (TurnHudClass)
    {
        TurnHudWidget = CreateWidget<UTurnHudWidget>(this, TurnHudClass);
        if (TurnHudWidget)
        {
            TurnHudWidget->AddToViewport();
        }
    }
}

void ATBPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // Safety check on InputComponent
    if (!InputComponent)
    {
        return;
    }
    
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &ATBPlayerController::OnLeftClick);
    InputComponent->BindAction("RightClick", IE_Pressed, this, &ATBPlayerController::OnRightClick);
    InputComponent->BindAction("Confirm", IE_Pressed, this, &ATBPlayerController::OnConfirmPlacement);
    InputComponent->BindAction("CancelPreview", IE_Pressed, this, &ATBPlayerController::OnCancelPreview);
}

bool ATBPlayerController::TraceClick(FHitResult& OutHit) const
{
    FVector2D MousePos;
    if (!GetMousePosition(MousePos.X, MousePos.Y)) return false;
    return GetHitResultAtScreenPosition(MousePos, ECC_Visibility, true, OutHit);
}

AUnitCharacter* ATBPlayerController::GetUnitUnderCursor() const
{
    FHitResult Hit;
    if (!TraceClick(Hit)) return nullptr;
    return Cast<AUnitCharacter>(Hit.GetActor());
}

AGridTile* ATBPlayerController::GetTileUnderCursor() const
{
    FHitResult Hit;
    if (!TraceClick(Hit)) return nullptr;
    return Cast<AGridTile>(Hit.GetActor());
}

void ATBPlayerController::OnLeftClick()
{
    AUnitCharacter* HitUnit = GetUnitUnderCursor();
    if (HitUnit)
    {
        // Select unit (only allow selecting player units in player turn in Blueprint/game logic)
        SelectedUnit = HitUnit;

        // Update HUD target if present
        if (TurnHudWidget)
        {
            TurnHudWidget->SetTargetUnit(SelectedUnit);
        }
    }
    else
    {
        // Could be used for other interactions
    }
}

void ATBPlayerController::OnRightClick()
{
    if (!SelectedUnit) return;

    AGridTile* Tile = GetTileUnderCursor();
    if (!Tile) return;

    // Use cached GridManager; fallback to iterator if cache missing
    AGridManager* GM = CachedGridManager;
    if (!GM)
    {
        for (TActorIterator<AGridManager> It(GetWorld()); It; ++It)
        {
            if (IsValid(*It))
            {
                GM = *It;
                CachedGridManager = GM;
                break;
            }
        }
    }
    
    if (GM)
    {
        TArray<AGridTile*> Path = GM->FindPath(SelectedUnit->CurrentTile, Tile);
        if (Path.Num())
        {
            // Request preview move (visual only); confirmation happens via Confirm input/UI
            SelectedUnit->RequestPreviewMove(Path);
        }
    }
}

void ATBPlayerController::OnConfirmPlacement()
{
    if (!SelectedUnit) return;
    SelectedUnit->ConfirmPlacement();

    // After confirm, update HUD stats
    if (TurnHudWidget)
    {
        TurnHudWidget->RefreshAllStats();
    }
}

void ATBPlayerController::OnCancelPreview()
{
    if (!SelectedUnit) return;
    SelectedUnit->CancelPreviewMove();

    if (TurnHudWidget)
    {
        TurnHudWidget->RefreshAllStats();
    }
}

void ATBPlayerController::OnCastMagicArrow()
{
    if (!SelectedUnit) return;
    AGridTile* Tile = GetTileUnderCursor();
    if (!Tile) return;
    SelectedUnit->CastAbilityAtTile(FName("MagicArrow"), Tile);

    if (TurnHudWidget)
    {
        TurnHudWidget->RefreshAllStats();
    }
}

void ATBPlayerController::OnCastBoulder()
{
    if (!SelectedUnit) return;
    AGridTile* Tile = GetTileUnderCursor();
    if (!Tile) return;
    SelectedUnit->CastAbilityAtTile(FName("Boulder"), Tile);

    if (TurnHudWidget)
    {
        TurnHudWidget->RefreshAllStats();
    }
}
