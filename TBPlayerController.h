#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBPlayerController.generated.h"

class AGridManager;
class AUnitCharacter;
class AGridTile;
class UTurnHudWidget;

UCLASS()
class DENEME_API ATBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ATBPlayerController();

    virtual void SetupInputComponent() override;

    // UI widget class for Turn HUD (assign in editor)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UTurnHudWidget> TurnHudClass;

    UPROPERTY()
    UTurnHudWidget* TurnHudWidget;

    // Expose selected unit publicly so UI code can use it
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selection")
    AUnitCharacter* SelectedUnit;

    // Helper cast functions to be bound to UI buttons
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void OnCastMagicArrow();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void OnCastBoulder();

    // Confirm placement helper (also callable by HUD)
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void OnConfirmPlacement();

protected:
    virtual void BeginPlay() override;

    // Cached GridManager pointer to avoid iterating actors on each right-click
    UPROPERTY()
    AGridManager* CachedGridManager;

    // Click handling
    void OnLeftClick();
    void OnRightClick();

    // Cancel preview (optional, bind to key)
    void OnCancelPreview();

    // Helpers
    bool TraceClick(FHitResult& OutHit) const;
    AGridTile* GetTileUnderCursor() const;
    AUnitCharacter* GetUnitUnderCursor() const;
};

