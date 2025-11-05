#include "AGridManager.h"
#include "AGridTile.h"
#include "Engine/World.h"

AGridManager::AGridManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGridManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Optionally auto-generate grid on BeginPlay
    // Uncomment if you want automatic generation
    // if (TileClass) GenerateGrid();
}

void AGridManager::GenerateGrid()
{
    if (!TileClass) return;
    
    // Clear existing tiles
    for (AGridTile* Tile : Tiles)
    {
        if (Tile) Tile->Destroy();
    }
    Tiles.Empty();
    
    // Spawn tiles
    FVector BaseLocation = GetActorLocation();
    for (int32 Y = 0; Y < GridHeight; ++Y)
    {
        for (int32 X = 0; X < GridWidth; ++X)
        {
            FVector TileLocation = BaseLocation + FVector(X * TileSize, Y * TileSize, 0.0f);
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            
            AGridTile* NewTile = GetWorld()->SpawnActor<AGridTile>(TileClass, TileLocation, FRotator::ZeroRotator, SpawnParams);
            if (NewTile)
            {
                NewTile->X = X;
                NewTile->Y = Y;
                Tiles.Add(NewTile);
            }
        }
    }
}

AGridTile* AGridManager::GetTileAt(int32 X, int32 Y) const
{
    if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridHeight) return nullptr;
    int32 Index = Y * GridWidth + X;
    if (Index < 0 || Index >= Tiles.Num()) return nullptr;
    return Tiles[Index];
}

TArray<AGridTile*> AGridManager::GetNeighbors(AGridTile* Tile) const
{
    TArray<AGridTile*> Neighbors;
    if (!Tile) return Neighbors;
    
    // Four directions: up, down, left, right
    AGridTile* Up = GetTileAt(Tile->X, Tile->Y - 1);
    AGridTile* Down = GetTileAt(Tile->X, Tile->Y + 1);
    AGridTile* Left = GetTileAt(Tile->X - 1, Tile->Y);
    AGridTile* Right = GetTileAt(Tile->X + 1, Tile->Y);
    
    if (Up && Up->bIsWalkable) Neighbors.Add(Up);
    if (Down && Down->bIsWalkable) Neighbors.Add(Down);
    if (Left && Left->bIsWalkable) Neighbors.Add(Left);
    if (Right && Right->bIsWalkable) Neighbors.Add(Right);
    
    return Neighbors;
}

int32 AGridManager::GetManhattanDistance(AGridTile* A, AGridTile* B) const
{
    if (!A || !B) return 0;
    return FMath::Abs(A->X - B->X) + FMath::Abs(A->Y - B->Y);
}

TArray<AGridTile*> AGridManager::FindPath(AGridTile* Start, AGridTile* End) const
{
    TArray<AGridTile*> Path;
    if (!Start || !End) return Path;
    if (Start == End) return Path;
    
    // A* pathfinding implementation
    TArray<FPathNode*> OpenSet;
    TArray<FPathNode*> ClosedSet;
    TMap<AGridTile*, FPathNode*> NodeMap;
    
    // Create start node
    FPathNode* StartNode = new FPathNode();
    StartNode->Tile = Start;
    StartNode->GCost = 0;
    StartNode->HCost = GetManhattanDistance(Start, End);
    StartNode->Parent = nullptr;
    
    OpenSet.Add(StartNode);
    NodeMap.Add(Start, StartNode);
    
    FPathNode* CurrentNode = nullptr;
    
    while (OpenSet.Num() > 0)
    {
        // Find node with lowest FCost
        CurrentNode = OpenSet[0];
        for (FPathNode* Node : OpenSet)
        {
            if (Node->FCost() < CurrentNode->FCost() || 
                (Node->FCost() == CurrentNode->FCost() && Node->HCost < CurrentNode->HCost))
            {
                CurrentNode = Node;
            }
        }
        
        OpenSet.Remove(CurrentNode);
        ClosedSet.Add(CurrentNode);
        
        // Found the end
        if (CurrentNode->Tile == End)
        {
            break;
        }
        
        // Check neighbors
        TArray<AGridTile*> Neighbors = GetNeighbors(CurrentNode->Tile);
        for (AGridTile* NeighborTile : Neighbors)
        {
            // Skip if occupied (unless it's the destination)
            if (NeighborTile->Occupant && NeighborTile != End)
            {
                continue;
            }
            
            // Check if in closed set
            bool bInClosedSet = false;
            for (FPathNode* ClosedNode : ClosedSet)
            {
                if (ClosedNode->Tile == NeighborTile)
                {
                    bInClosedSet = true;
                    break;
                }
            }
            if (bInClosedSet) continue;
            
            int32 NewGCost = CurrentNode->GCost + NeighborTile->MovementCost;
            
            FPathNode* NeighborNode = nullptr;
            if (NodeMap.Contains(NeighborTile))
            {
                NeighborNode = NodeMap[NeighborTile];
            }
            
            if (!NeighborNode || NewGCost < NeighborNode->GCost)
            {
                if (!NeighborNode)
                {
                    NeighborNode = new FPathNode();
                    NeighborNode->Tile = NeighborTile;
                    NodeMap.Add(NeighborTile, NeighborNode);
                    OpenSet.Add(NeighborNode);
                }
                
                NeighborNode->GCost = NewGCost;
                NeighborNode->HCost = GetManhattanDistance(NeighborTile, End);
                NeighborNode->Parent = CurrentNode;
            }
        }
    }
    
    // Reconstruct path
    if (CurrentNode && CurrentNode->Tile == End)
    {
        FPathNode* Node = CurrentNode;
        while (Node)
        {
            Path.Insert(Node->Tile, 0);
            Node = Node->Parent;
        }
    }
    
    // Cleanup
    for (auto& Pair : NodeMap)
    {
        delete Pair.Value;
    }
    
    return Path;
}
