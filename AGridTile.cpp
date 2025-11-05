#include "AGridTile.h"
#include "Components/SceneComponent.h"

AGridTile::AGridTile()
{
    PrimaryActorTick.bCanEverTick = false;
    
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void AGridTile::BeginPlay()
{
    Super::BeginPlay();
}

FVector AGridTile::GetTileCenter() const
{
    // Return the actor's location as the tile center
    // In a real implementation, you might want to offset by half the tile size
    return GetActorLocation();
}

bool AGridTile::IsAvailable() const
{
    return bIsWalkable && (Occupant == nullptr);
}
