#include "TurnStatsComponent.h"

UTurnStatsComponent::UTurnStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UTurnStatsComponent::SpendMovement(int32 Cost)
{
    if (Cost < 0) return false;
    if (MovementPoints < Cost) return false;
    
    MovementPoints -= Cost;
    OnStatsChanged.Broadcast();
    return true;
}

bool UTurnStatsComponent::SpendAction(int32 Cost)
{
    if (Cost < 0) return false;
    if (ActionPoints < Cost) return false;
    
    ActionPoints -= Cost;
    OnStatsChanged.Broadcast();
    return true;
}

void UTurnStatsComponent::ResetForNewTurn()
{
    MovementPoints = MaxMovementPoints;
    ActionPoints = MaxActionPoints;
    OnStatsChanged.Broadcast();
}
