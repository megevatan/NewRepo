#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitCharacter.generated.h"

class UTurnStatsComponent;
class AGridTile;
class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHPChanged, int32, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDied);

USTRUCT(BlueprintType)
struct FAbilityData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FName AbilityName = NAME_None;

    // Range in tiles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    int32 Range = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    int32 MinDamage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    int32 MaxDamage = 0;

    // AP cost
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    int32 APCost = 1;

    // Maximum casts per turn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    int32 MaxCastsPerTurn = 1;

    // Remaining casts for current turn (runtime)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
    int32 CastsRemaining = 0;

    // Whether damage is magical (placeholder for resistances later)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    bool bIsMagical = false;

    FAbilityData() {}
};

UCLASS()
class DENEME_API AUnitCharacter : public AActor
{
    GENERATED_BODY()

public:
    AUnitCharacter();

    // Root for visuals (Blueprints should add a SkeletalMesh + Camera here)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turn")
    UTurnStatsComponent* TurnStats;

    // Current committed tile (occupant is set when placement is confirmed)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    AGridTile* CurrentTile;

    // HP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MaxHP = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 HP = 100;

    // Death VFX to spawn on death (optional)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    UParticleSystem* DeathEffect;

    virtual void BeginPlay() override;

    // Preview move: snap visually to the destination (no MP deducted, CurrentTile unchanged).
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool RequestPreviewMove(const TArray<AGridTile*>& Path);

    // Cancel the preview and snap back to committed tile
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void CancelPreviewMove();

    // Confirm placement after preview: deduct MP and commit occupancy/current tile.
    UFUNCTION(BlueprintCallable, Category = "Turn")
    void ConfirmPlacement();

    // Returns how many movement points the preview path will cost (1 per tile)
    UFUNCTION(BlueprintCallable, Category = "Movement")
    int32 GetPreviewCost() const;

    // Abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    FAbilityData MagicArrow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    FAbilityData Boulder;

    // Attempt to cast an ability at the target tile. Returns true if cast succeeded.
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool CastAbilityAtTile(FName AbilityName, AGridTile* TargetTile);

    // Receive damage (applies to HP, calls OnDeath if <= 0)
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ReceiveDamage(int32 Amount, bool bMagical);

    // Reset per-turn values on the unit (MP/AP and ability counters)
    UFUNCTION(BlueprintCallable, Category = "Turn")
    void ResetForNewTurn();

    // Delegates for UI
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHPChanged OnHPChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDied OnDied;

protected:
    // Preview state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsPreviewing = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    TArray<AGridTile*> PreviewPath;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    int32 PreviewCost = 0;

    // Original transform to restore if preview cancelled
    FVector OriginalLocation;
    AGridTile* OriginalTile = nullptr;

    // Helper to visually snap actor to the center of a tile (does not change occupancy)
    void SnapToTileVisual(AGridTile* Tile);

    // Helper to commit change of occupancy/current tile
    void CommitToTile(AGridTile* Tile);

    // Apply the ability effects to the occupant on a tile (if any)
    bool ApplyAbilityToTile(const FAbilityData& Ability, AGridTile* Tile);

    // Handle death (cleans up occupancy, broadcasts, spawns VFX)
    void OnDeath();
};

