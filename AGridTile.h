#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGridTile.generated.h"

UCLASS()
class DENEME_API AGridTile : public AActor
{
    GENERATED_BODY()
    
public:
    AGridTile();

    // Visual component (can be a StaticMesh in Blueprint)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;
    
    // Grid coordinates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 X = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 Y = 0;
    
    // Whether this tile is walkable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bIsWalkable = true;
    
    // Actor currently occupying this tile
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    AActor* Occupant = nullptr;
    
    // Cost to move into this tile (default 1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 MovementCost = 1;
    
    // Get the center position of this tile (for unit placement)
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector GetTileCenter() const;
    
    // Check if this tile is available (walkable and unoccupied)
    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsAvailable() const;

protected:
    virtual void BeginPlay() override;
};
