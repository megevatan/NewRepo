#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnHudWidget.generated.h"

class UTextBlock;
class UButton;
class AUnitCharacter;

UCLASS()
class DENEME_API UTurnHudWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // BindWidget requires the UMG designer elements to be named exactly these names
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TurnTimerText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TurnStateText;

    // Unit stats
    UPROPERTY(meta = (BindWidget))
    UTextBlock* UnitHPText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UnitMPText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* UnitAPText;

    UPROPERTY(meta = (BindWidget))
    UButton* EndTurnButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CastMagicArrowButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CastBoulderButton;

    // Set which unit the HUD should display (binds delegates)
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetTargetUnit(AUnitCharacter* Unit);

    // Refresh (public helper)
    UFUNCTION(BlueprintCallable, Category = "UI")
    void RefreshAllStats();

protected:
    UPROPERTY()
    AUnitCharacter* BoundUnit = nullptr;

    UFUNCTION()
    void HandleEndTurnClicked();

    UFUNCTION()
    void HandleConfirmClicked();

    UFUNCTION()
    void HandleCastMagicArrowClicked();

    UFUNCTION()
    void HandleCastBoulderClicked();

    UFUNCTION()
    void OnUnitHPChanged(int32 NewHP);

    UFUNCTION()
    void OnUnitDied();

    UFUNCTION()
    void OnUnitStatsChanged();
};

