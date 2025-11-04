#include "UnitCharacter.h"
#include "TurnStatsComponent.h"
#include "AGridTile.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AUnitCharacter::AUnitCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    TurnStats = CreateDefaultSubobject<UTurnStatsComponent>(TEXT("TurnStats"));
    HP = MaxHP;

    // Default ability setup
    MagicArrow.AbilityName = FName("MagicArrow");
    MagicArrow.Range = 12;
    MagicArrow.MinDamage = 5;
    MagicArrow.MaxDamage = 10;
    MagicArrow.APCost = 3;
    MagicArrow.MaxCastsPerTurn = 2;
    MagicArrow.bIsMagical = true;
    MagicArrow.CastsRemaining = MagicArrow.MaxCastsPerTurn;

    Boulder.AbilityName = FName("Boulder");
    Boulder.Range = 1;
    Boulder.MinDamage = 10;
    Boulder.MaxDamage = 12;
    Boulder.APCost = 2;
    Boulder.MaxCastsPerTurn = 3;
    Boulder.bIsMagical = false;
    Boulder.CastsRemaining = Boulder.MaxCastsPerTurn;
}

void AUnitCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (CurrentTile)
    {
        // Ensure occupant set if not already (committed state)
        CurrentTile->Occupant = this;
        SnapToTileVisual(CurrentTile);
    }
}

void AUnitCharacter::SnapToTileVisual(AGridTile* Tile)
{
    if (!Tile) return;
    SetActorLocation(Tile->GetTileCenter());
}

bool AUnitCharacter::RequestPreviewMove(const TArray<AGridTile*>& Path)
{
    if (Path.Num() < 2) return false; // 0 or 1 means no movement
    if (!TurnStats) return false;

    // Compute cost (1 per step)
    int32 Cost = Path.Num() - 1;

    // Save original location/tile for cancellation
    if (!bIsPreviewing)
    {
        OriginalLocation = GetActorLocation();
        OriginalTile = CurrentTile;
    }

    // Record preview path/cost
    PreviewPath = Path;
    PreviewCost = Cost;
    bIsPreviewing = true;

    // Snap visually to final tile center (do NOT change CurrentTile or Occupant)
    AGridTile* Dest = PreviewPath.Last();
    SnapToTileVisual(Dest);

    // Mark as not confirmed until confirmed by the player (UI/Confirm button)
    if (TurnStats)
    {
        // mark unconfirmed
    }

    return true;
}

void AUnitCharacter::CancelPreviewMove()
{
    if (!bIsPreviewing) return;

    // Snap back to original committed tile (or stored location)
    if (OriginalTile)
    {
        SnapToTileVisual(OriginalTile);
    }
    else
    {
        SetActorLocation(OriginalLocation);
    }

    PreviewPath.Empty();
    PreviewCost = 0;
    bIsPreviewing = false;

    // notify UI if needed (no MP change on cancel)
}

void AUnitCharacter::ConfirmPlacement()
{
    if (!bIsPreviewing || PreviewPath.Num() < 2)
    {
        // Nothing to confirm (still at committed tile)
        return;
    }

    // Check cost and spend MP
    if (!TurnStats)
    {
        CancelPreviewMove();
        return;
    }

    if (!TurnStats->SpendMovement(PreviewCost))
    {
        // Not enough MP -> revert
        CancelPreviewMove();
        return;
    }

    // Commit occupancy change: clear old tile occupant and set new occupant
    AGridTile* Dest = PreviewPath.Last();
    if (CurrentTile && CurrentTile->Occupant == this)
    {
        CurrentTile->Occupant = nullptr;
    }

    // If destination is occupied by someone else unexpectedly, fail and revert
    if (Dest->Occupant && Dest->Occupant != this)
    {
        // revert and refund the spent MP
        TurnStats->MovementPoints += PreviewCost;
        CancelPreviewMove();
        return;
    }

    CommitToTile(Dest);

    // Clear preview state
    PreviewPath.Empty();
    PreviewCost = 0;
    bIsPreviewing = false;

    // notify UI about MP change (via TurnStats or events)
}

int32 AUnitCharacter::GetPreviewCost() const
{
    return PreviewCost;
}

void AUnitCharacter::CommitToTile(AGridTile* Tile)
{
    if (!Tile) return;
    CurrentTile = Tile;
    Tile->Occupant = this;
    SnapToTileVisual(Tile);
}

bool AUnitCharacter::ApplyAbilityToTile(const FAbilityData& Ability, AGridTile* Tile)
{
    if (!Tile) return false;
    AActor* Occ = Tile->Occupant;
    if (!Occ) return false; // nothing to hit

    // Look for AUnitCharacter on the occupant
    AUnitCharacter* TargetUnit = Cast<AUnitCharacter>(Occ);
    if (!TargetUnit) return false;

    // Random damage in range
    int32 Damage = FMath::RandRange(Ability.MinDamage, Ability.MaxDamage);
    TargetUnit->ReceiveDamage(Damage, Ability.bIsMagical);

    return true;
}

bool AUnitCharacter::CastAbilityAtTile(FName AbilityName, AGridTile* TargetTile)
{
    if (!TurnStats) return false;
    if (!TargetTile) return false;

    // Find ability by name
    FAbilityData* Chosen = nullptr;
    if (AbilityName == MagicArrow.AbilityName || AbilityName.IsNone() || AbilityName == FName("MagicArrow"))
    {
        Chosen = &MagicArrow;
    }
    else if (AbilityName == Boulder.AbilityName || AbilityName == FName("Boulder"))
    {
        Chosen = &Boulder;
    }
    else
    {
        if (AbilityName == FName("MagicArrow")) Chosen = &MagicArrow;
        else if (AbilityName == FName("Boulder")) Chosen = &Boulder;
    }

    if (!Chosen) return false;

    // Check casts remaining
    if (Chosen->CastsRemaining <= 0) return false;

    // Check AP
    if (TurnStats->ActionPoints < Chosen->APCost) return false;

    // Range check (Manhattan) relative to current committed tile (or preview dest if previewing)
    AGridTile* OriginTile = CurrentTile;
    if (!OriginTile && bIsPreviewing && PreviewPath.Num()) OriginTile = PreviewPath.Last();
    if (!OriginTile) return false;

    int32 Dist = FMath::Abs(OriginTile->X - TargetTile->X) + FMath::Abs(OriginTile->Y - TargetTile->Y);
    if (Dist > Chosen->Range) return false;

    // Apply effect (if something to hit)
    bool bApplied = ApplyAbilityToTile(*Chosen, TargetTile);

    // Consume AP and a cast
    TurnStats->SpendAction(Chosen->APCost);
    Chosen->CastsRemaining = FMath::Max(0, Chosen->CastsRemaining - 1);

    // Notify UI about AP/ability changes (delegate or TurnStats)
    return bApplied;
}

void AUnitCharacter::ReceiveDamage(int32 Amount, bool bMagical)
{
    if (Amount <= 0) return;
    HP -= Amount;
    HP = FMath::Max(0, HP);

    // Broadcast HP changed
    OnHPChanged.Broadcast(HP);

    if (HP <= 0)
    {
        OnDeath();
    }
}

void AUnitCharacter::OnDeath()
{
    // Clean up occupancy pointer to avoid dangling refs
    if (CurrentTile && CurrentTile->Occupant == this)
    {
        CurrentTile->Occupant = nullptr;
    }

    // Spawn death VFX if assigned
    if (DeathEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation(), GetActorRotation(), true);
    }

    // Broadcast death event (UI/other systems can bind)
    OnDied.Broadcast();

    // Destroy actor
    Destroy();
}

void AUnitCharacter::ResetForNewTurn()
{
    if (TurnStats)
    {
        TurnStats->ResetForNewTurn();
    }
    // Reset ability cast counters
    MagicArrow.CastsRemaining = MagicArrow.MaxCastsPerTurn;
    Boulder.CastsRemaining = Boulder.MaxCastsPerTurn;
}

