#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGridManager.generated.h"

class AGridTile;

UCLASS()
class DENEME_API AGridManager : public AActor
{
    GENERATED_BODY()
    
public:
    AGridManager();

    // Grid dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridWidth = 10;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 GridHeight = 10;
    
    // Tile spacing (distance between tile centers)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float TileSize = 100.0f;
    
    // Tile class to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSubclassOf<AGridTile> TileClass;
    
    // All tiles in the grid (2D array stored as 1D)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    TArray<AGridTile*> Tiles;
    
    // Generate the grid at runtime
    UFUNCTION(BlueprintCallable, Category = "Grid")
    void GenerateGrid();
    
    // Get tile at specific coordinates
    UFUNCTION(BlueprintCallable, Category = "Grid")
    AGridTile* GetTileAt(int32 X, int32 Y) const;
    
    // Get neighboring tiles (up, down, left, right)
    UFUNCTION(BlueprintCallable, Category = "Grid")
    TArray<AGridTile*> GetNeighbors(AGridTile* Tile) const;
    
    // Find path between two tiles using A* algorithm
    UFUNCTION(BlueprintCallable, Category = "Grid")
    TArray<AGridTile*> FindPath(AGridTile* Start, AGridTile* End) const;
    
    // Calculate Manhattan distance between two tiles
    UFUNCTION(BlueprintCallable, Category = "Grid")
    int32 GetManhattanDistance(AGridTile* A, AGridTile* B) const;

protected:
    virtual void BeginPlay() override;
    
private:
    // Helper for A* pathfinding
    struct FPathNode
    {
        AGridTile* Tile;
        FPathNode* Parent;
        int32 GCost; // Distance from start
        int32 HCost; // Heuristic distance to end
        int32 FCost() const { return GCost + HCost; }
        
        FPathNode() : Tile(nullptr), Parent(nullptr), GCost(0), HCost(0) {}
    };
};
