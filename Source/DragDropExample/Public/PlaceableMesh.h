#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableMesh.generated.h"

/*
* A placeable mesh that can be dragged and dropped in the world.
*/
UCLASS()
class DRAGDROPEXAMPLE_API APlaceableMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaceableMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// The mesh component for this placeable mesh.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	// Maximum angle from vertical on which this actor can be placed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	float MaxPlacementAngle;

	// Last position of the placeable mesh, used for when the linetrace misses or hits an invalid placement location.
	UPROPERTY(BlueprintReadWrite, Category="Position")
	FVector LastPosition;

};
