#include "AGridTile.h"

AGridTile::AGridTile()
{
    PrimaryActorTick.bCanEverTick = false;
}

FVector AGridTile::GetTileCenter() const
{
    return GetActorLocation();
}
