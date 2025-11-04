#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnStatsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DENEME_API UTurnStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTurnStatsComponent();

    // Maximum movement points per turn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnStats")
    int32 MaxMovementPoints = 5;

    // Current available movement points
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TurnStats")
    int32 MovementPoints = 5;

    // Maximum action points per turn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnStats")
    int32 MaxActionPoints = 10;

    // Current available action points
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TurnStats")
    int32 ActionPoints = 10;

    // Spend movement points - returns true if successful
    UFUNCTION(BlueprintCallable, Category = "TurnStats")
    bool SpendMovement(int32 Amount);

    // Refund movement points
    UFUNCTION(BlueprintCallable, Category = "TurnStats")
    void RefundMovement(int32 Amount);

    // Spend action points - returns true if successful
    UFUNCTION(BlueprintCallable, Category = "TurnStats")
    bool SpendAction(int32 Amount);

    // Refund action points
    UFUNCTION(BlueprintCallable, Category = "TurnStats")
    void RefundAction(int32 Amount);

    // Reset stats for a new turn
    UFUNCTION(BlueprintCallable, Category = "TurnStats")
    void ResetForNewTurn();

protected:
    virtual void BeginPlay() override;
};
