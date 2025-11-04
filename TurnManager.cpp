#include "TurnManager.h"
#include "UnitCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"

ATurnManager::ATurnManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATurnManager::BeginPlay()
{
    Super::BeginPlay();
    CollectUnits();
}

void ATurnManager::CollectUnits()
{
    AllUnits.Empty();
    for (TActorIterator<AUnitCharacter> It(GetWorld()); It; ++It)
    {
        AUnitCharacter* Unit = *It;
        if (IsValid(Unit))
        {
            AllUnits.Add(Unit);
        }
    }
}

void ATurnManager::ResetAllUnitsForNewTurn()
{
    for (AUnitCharacter* Unit : AllUnits)
    {
        if (Unit)
        {
            Unit->ResetForNewTurn();
        }
    }
}

void ATurnManager::StartNextTurn()
{
    ResetAllUnitsForNewTurn();
    // Additional turn start logic can be added here
}

void ATurnManager::EndPlayerTurn()
{
    // Logic for ending the player's turn
    // This could transition to enemy turn or next player
    StartNextTurn();
}

ATurnManager* ATurnManager::GetTurnManager(UWorld* World)
{
    if (!World)
    {
        return nullptr;
    }
    
    for (TActorIterator<ATurnManager> It(World); It; ++It)
    {
        if (IsValid(*It))
        {
            return *It;
        }
    }
    
    return nullptr;
}
