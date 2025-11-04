#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

class AGridTile;

UCLASS()
class DENEME_API AGridManager : public AActor
{
    GENERATED_BODY()

public:
    AGridManager();

    virtual void BeginPlay() override;

    // Find a path from start to goal using BFS orthogonal pathfinding
    // Returns an array including start and goal tiles (or empty if no path)
    UFUNCTION(BlueprintCallable, Category = "Grid")
    TArray<AGridTile*> FindPath(AGridTile* Start, AGridTile* Goal);

protected:
    // Map of (X,Y) coordinates to tiles
    UPROPERTY()
    TMap<FIntPoint, AGridTile*> TileMap;

    // Discover all AGridTile actors in the level and populate TileMap
    void DiscoverTiles();
};
