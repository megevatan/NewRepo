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

    // Grid coordinates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 X = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 Y = 0;

    // Current occupant (nullptr if empty)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
    AActor* Occupant = nullptr;

    // Get the center position of this tile for unit placement
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector GetTileCenter() const;
};
