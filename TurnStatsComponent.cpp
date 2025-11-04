#include "TurnStatsComponent.h"

UTurnStatsComponent::UTurnStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTurnStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    // Initialize current values to max
    MovementPoints = MaxMovementPoints;
    ActionPoints = MaxActionPoints;
}

bool UTurnStatsComponent::SpendMovement(int32 Amount)
{
    if (Amount < 0 || MovementPoints < Amount)
    {
        return false;
    }
    
    MovementPoints -= Amount;
    return true;
}

void UTurnStatsComponent::RefundMovement(int32 Amount)
{
    if (Amount < 0)
    {
        return;
    }
    
    MovementPoints = FMath::Min(MovementPoints + Amount, MaxMovementPoints);
}

bool UTurnStatsComponent::SpendAction(int32 Amount)
{
    if (Amount < 0 || ActionPoints < Amount)
    {
        return false;
    }
    
    ActionPoints -= Amount;
    return true;
}

void UTurnStatsComponent::RefundAction(int32 Amount)
{
    if (Amount < 0)
    {
        return;
    }
    
    ActionPoints = FMath::Min(ActionPoints + Amount, MaxActionPoints);
}

void UTurnStatsComponent::ResetForNewTurn()
{
    MovementPoints = MaxMovementPoints;
    ActionPoints = MaxActionPoints;
}
