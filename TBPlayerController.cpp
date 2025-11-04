#include "TBPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UnitCharacter.h"
#include "AGridTile.h"
#include "AGridManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TurnHudWidget.h"

ATBPlayerController::ATBPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ATBPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (TurnHudClass)
    {
        TurnHudWidget = CreateWidget<UUserWidget>(this, TurnHudClass);
        if (TurnHudWidget)
        {
            TurnHudWidget->AddToViewport();
        }
    }
}

void ATBPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
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
            UTurnHudWidget* Hud = Cast<UTurnHudWidget>(TurnHudWidget);
            if (Hud)
            {
                Hud->SetTargetUnit(SelectedUnit);
            }
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

    // Ask GridManager to find a path
    for (TActorIterator<AGridManager> It(GetWorld()); It; ++It)
    {
        AGridManager* GM = *It;
        if (!GM) continue;
        TArray<AGridTile*> Path = GM->FindPath(SelectedUnit->CurrentTile, Tile);
        if (Path.Num())
        {
            // Request preview move (visual only); confirmation happens via Confirm input/UI
            SelectedUnit->RequestPreviewMove(Path);
        }
        break;
    }
}

void ATBPlayerController::OnConfirmPlacement()
{
    if (!SelectedUnit) return;
    SelectedUnit->ConfirmPlacement();

    // After confirm, update HUD stats
    if (TurnHudWidget)
    {
        UTurnHudWidget* Hud = Cast<UTurnHudWidget>(TurnHudWidget);
        if (Hud)
        {
            Hud->RefreshAllStats();
        }
    }
}

void ATBPlayerController::OnCancelPreview()
{
    if (!SelectedUnit) return;
    SelectedUnit->CancelPreviewMove();

    if (TurnHudWidget)
    {
        UTurnHudWidget* Hud = Cast<UTurnHudWidget>(TurnHudWidget);
        if (Hud) Hud->RefreshAllStats();
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
        UTurnHudWidget* Hud = Cast<UTurnHudWidget>(TurnHudWidget);
        if (Hud) Hud->RefreshAllStats();
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
        UTurnHudWidget* Hud = Cast<UTurnHudWidget>(TurnHudWidget);
        if (Hud) Hud->RefreshAllStats();
    }
}
