#include "TurnHudWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TBPlayerController.h"
#include "UnitCharacter.h"

void UTurnHudWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (EndTurnButton)
    {
        EndTurnButton->OnClicked.AddDynamic(this, &UTurnHudWidget::HandleEndTurnClicked);
    }
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UTurnHudWidget::HandleConfirmClicked);
    }
    if (CastMagicArrowButton)
    {
        CastMagicArrowButton->OnClicked.AddDynamic(this, &UTurnHudWidget::HandleCastMagicArrowClicked);
    }
    if (CastBoulderButton)
    {
        CastBoulderButton->OnClicked.AddDynamic(this, &UTurnHudWidget::HandleCastBoulderClicked);
    }

    if (TurnTimerText) TurnTimerText->SetText(FText::FromString("60"));
    if (TurnStateText) TurnStateText->SetText(FText::FromString("Idle"));
    RefreshAllStats();
}

void UTurnHudWidget::SetTargetUnit(AUnitCharacter* Unit)
{
    // Unbind previous
    if (BoundUnit)
    {
        BoundUnit->OnHPChanged.RemoveAll(this);
        BoundUnit->OnDied.RemoveAll(this);
    }

    BoundUnit = Unit;

    if (BoundUnit)
    {
        BoundUnit->OnHPChanged.AddDynamic(this, &UTurnHudWidget::OnUnitHPChanged);
        BoundUnit->OnDied.AddDynamic(this, &UTurnHudWidget::OnUnitDied);
    }

    RefreshAllStats();
}

void UTurnHudWidget::HandleEndTurnClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ATBPlayerController* TBPC = Cast<ATBPlayerController>(PC);
    if (TBPC)
    {
        // Expect TBPC or level script to call TurnManager->EndPlayerTurn
    }
}

void UTurnHudWidget::HandleConfirmClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ATBPlayerController* TBPC = Cast<ATBPlayerController>(PC);
    if (TBPC)
    {
        TBPC->OnConfirmPlacement();
    }
}

void UTurnHudWidget::HandleCastMagicArrowClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ATBPlayerController* TBPC = Cast<ATBPlayerController>(PC);
    if (TBPC && TBPC->SelectedUnit)
    {
        TBPC->OnCastMagicArrow();
    }
}

void UTurnHudWidget::HandleCastBoulderClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ATBPlayerController* TBPC = Cast<ATBPlayerController>(PC);
    if (TBPC && TBPC->SelectedUnit)
    {
        TBPC->OnCastBoulder();
    }
}

void UTurnHudWidget::OnUnitHPChanged(int32 NewHP)
{
    if (UnitHPText)
    {
        UnitHPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d/%d"), NewHP, BoundUnit ? BoundUnit->MaxHP : 0)));
    }
}

void UTurnHudWidget::OnUnitDied()
{
    // Clear display for the dead unit
    RefreshAllStats();
}

void UTurnHudWidget::OnUnitStatsChanged()
{
    RefreshAllStats();
}

void UTurnHudWidget::RefreshAllStats()
{
    if (!BoundUnit)
    {
        if (UnitHPText) UnitHPText->SetText(FText::FromString("HP: -"));
        if (UnitMPText) UnitMPText->SetText(FText::FromString("MP: -"));
        if (UnitAPText) UnitAPText->SetText(FText::FromString("AP: -"));
        return;
    }

    if (UnitHPText) UnitHPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %d/%d"), BoundUnit->HP, BoundUnit->MaxHP)));
    if (UnitMPText)
    {
        if (BoundUnit->TurnStats)
            UnitMPText->SetText(FText::FromString(FString::Printf(TEXT("MP: %d/%d"), BoundUnit->TurnStats->MovementPoints, BoundUnit->TurnStats->MaxMovementPoints)));
        else
            UnitMPText->SetText(FText::FromString("MP: -"));
    }
    if (UnitAPText)
    {
        if (BoundUnit->TurnStats)
            UnitAPText->SetText(FText::FromString(FString::Printf(TEXT("AP: %d/%d"), BoundUnit->TurnStats->ActionPoints, BoundUnit->TurnStats->MaxActionPoints)));
        else
            UnitAPText->SetText(FText::FromString("AP: -"));
    }
}

