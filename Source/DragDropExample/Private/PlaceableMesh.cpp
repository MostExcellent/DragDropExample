#include "PlaceableMesh.h"

// Sets default values
APlaceableMesh::APlaceableMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void APlaceableMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceableMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LastPosition = GetActorLocation();
}

