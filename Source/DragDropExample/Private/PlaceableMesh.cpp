#include "PlaceableMesh.h"

APlaceableMesh::APlaceableMesh()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void APlaceableMesh::BeginPlay()
{
	Super::BeginPlay();
}

void APlaceableMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastPosition = GetActorLocation();
}
