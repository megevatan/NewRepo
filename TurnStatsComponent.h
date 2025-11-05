#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnStatsComponent.generated.h"

// Delegate for when MP/AP changes (for UI updates)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatsChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DENEME_API UTurnStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTurnStatsComponent();

    // Movement Points (MP) - for moving across tiles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Stats")
    int32 MovementPoints = 10;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Stats")
    int32 MaxMovementPoints = 10;
    
    // Action Points (AP) - for casting abilities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Stats")
    int32 ActionPoints = 5;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Stats")
    int32 MaxActionPoints = 5;
    
    // Spend movement points (returns true if successful)
    UFUNCTION(BlueprintCallable, Category = "Turn Stats")
    bool SpendMovement(int32 Cost);
    
    // Spend action points (returns true if successful)
    UFUNCTION(BlueprintCallable, Category = "Turn Stats")
    bool SpendAction(int32 Cost);
    
    // Reset stats for a new turn
    UFUNCTION(BlueprintCallable, Category = "Turn Stats")
    void ResetForNewTurn();
    
    // Event broadcast when stats change
    UPROPERTY(BlueprintAssignable, Category = "Turn Stats")
    FOnStatsChanged OnStatsChanged;
};
