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
	APlaceableMesh();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement", meta = (AllowPrivateAccess = "true"))
	float MaxPlacementAngle = 45.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Position", meta = (AllowPrivateAccess = "true"))
	FVector LastPosition = FVector::ZeroVector;

public:
	FORCEINLINE bool CanPlaceAngle(float Angle) const { return Angle <= MaxPlacementAngle; }
	FORCEINLINE FVector GetLastPosition() const { return LastPosition; }
};
