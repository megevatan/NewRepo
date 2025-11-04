#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnManager.generated.h"

class AUnitCharacter;

UCLASS()
class DENEME_API ATurnManager : public AActor
{
    GENERATED_BODY()

public:
    ATurnManager();

    virtual void BeginPlay() override;

    // Reset all units for a new turn
    UFUNCTION(BlueprintCallable, Category = "TurnManager")
    void ResetAllUnitsForNewTurn();

    // Start the next turn
    UFUNCTION(BlueprintCallable, Category = "TurnManager")
    void StartNextTurn();

    // End the player's turn
    UFUNCTION(BlueprintCallable, Category = "TurnManager")
    void EndPlayerTurn();

    // Get the global turn manager instance
    UFUNCTION(BlueprintCallable, Category = "TurnManager")
    static ATurnManager* GetTurnManager(UWorld* World);

protected:
    // All units managed by this turn manager
    UPROPERTY()
    TArray<AUnitCharacter*> AllUnits;

    // Collect all AUnitCharacter actors in the level
    void CollectUnits();
};
