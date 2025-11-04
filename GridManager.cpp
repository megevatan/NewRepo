#include "GridManager.h"
#include "AGridTile.h"
#include "EngineUtils.h"
#include "Engine/World.h"

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();
    DiscoverTiles();
}

void AGridManager::DiscoverTiles()
{
    TileMap.Empty();
    for (TActorIterator<AGridTile> It(GetWorld()); It; ++It)
    {
        AGridTile* Tile = *It;
        if (Tile)
        {
            FIntPoint Key(Tile->X, Tile->Y);
            TileMap.Add(Key, Tile);
        }
    }
}

TArray<AGridTile*> AGridManager::FindPath(AGridTile* Start, AGridTile* Goal)
{
    TArray<AGridTile*> EmptyPath;
    
    if (!Start || !Goal)
    {
        return EmptyPath;
    }
    
    if (Start == Goal)
    {
        // Same tile - return path with just that tile
        EmptyPath.Add(Start);
        return EmptyPath;
    }
    
    // BFS for orthogonal pathfinding
    TQueue<AGridTile*> Queue;
    TMap<AGridTile*, AGridTile*> CameFrom;
    
    Queue.Enqueue(Start);
    CameFrom.Add(Start, nullptr);
    
    bool bFoundGoal = false;
    
    while (!Queue.IsEmpty())
    {
        AGridTile* Current;
        Queue.Dequeue(Current);
        
        if (Current == Goal)
        {
            bFoundGoal = true;
            break;
        }
        
        // Check 4 orthogonal neighbors
        TArray<FIntPoint> Neighbors;
        Neighbors.Add(FIntPoint(Current->X + 1, Current->Y));
        Neighbors.Add(FIntPoint(Current->X - 1, Current->Y));
        Neighbors.Add(FIntPoint(Current->X, Current->Y + 1));
        Neighbors.Add(FIntPoint(Current->X, Current->Y - 1));
        
        for (const FIntPoint& NeighborCoord : Neighbors)
        {
            AGridTile** NeighborTilePtr = TileMap.Find(NeighborCoord);
            if (!NeighborTilePtr)
            {
                continue; // No tile at this coordinate
            }
            
            AGridTile* Neighbor = *NeighborTilePtr;
            
            // Skip if already visited
            if (CameFrom.Contains(Neighbor))
            {
                continue;
            }
            
            // Skip if occupied (unless it's the goal)
            if (Neighbor->Occupant && Neighbor != Goal)
            {
                continue;
            }
            
            Queue.Enqueue(Neighbor);
            CameFrom.Add(Neighbor, Current);
        }
    }
    
    if (!bFoundGoal)
    {
        return EmptyPath;
    }
    
    // Reconstruct path from goal to start
    TArray<AGridTile*> Path;
    AGridTile* Current = Goal;
    while (Current != nullptr)
    {
        Path.Add(Current);
        AGridTile** PrevPtr = CameFrom.Find(Current);
        Current = (PrevPtr ? *PrevPtr : nullptr);
    }
    
    // Reverse to get start-to-goal order
    Algo::Reverse(Path);
    
    return Path;
}
