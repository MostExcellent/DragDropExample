#include "DragDropController.h"
#include "CameraPawn.h"
#include "PlaceableMesh.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

ADragDropController::ADragDropController()
{
    PrimaryActorTick.bCanEverTick = true;

    PanSpeed = 400.0f;
    EdgePanMargin = 10.0f;
    ZoomSpeed = 100.0f;
    MinZoomDistance = 1000.0f;
    MaxZoomDistance = 3500.0f;
    TargetZoomAmount = 0.0f;
    bIsInPlacementMode = false;
    CurrentPlaceableMesh = nullptr;
    CurrentZoomAmount = 0.0f;
}

void ADragDropController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        CameraPawn = GetWorld()->SpawnActor<ACameraPawn>();
        Possess(CameraPawn);

        FVector InitialCameraLocation = FVector(0.0f, 0.0f, 2500.0f);
        FRotator InitialCameraRotation = FRotator(-45.0f, 0.0f, 0.0f);
        CameraPawn->SetActorLocation(InitialCameraLocation);
        CameraPawn->SetActorRotation(InitialCameraRotation);
    }

    EdgePanMargin = FMath::Clamp(EdgePanMargin, 0.0f, 50.0f);

    if (InputMappingContext)
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The map context is not set"));
    }

    bShowMouseCursor = true;
}

void ADragDropController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        Input->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &ADragDropController::ZoomIn);
        Input->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &ADragDropController::ZoomOut);
        Input->BindAction(SelectPlaceableOneAction, ETriggerEvent::Triggered, this, &ADragDropController::SelectPlaceableOne);
        Input->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &ADragDropController::LeftClick);
        Input->BindAction(CancelPlacementAction, ETriggerEvent::Triggered, this, &ADragDropController::CancelActorPlacement);
    }
}

void ADragDropController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    PanCamera(DeltaSeconds);
    ZoomCamera(DeltaSeconds);
    ProjectPlaceableMesh();
}

void ADragDropController::PanCamera(float DeltaSeconds)
{
    if (bIsInPlacementMode || !CameraPawn)
    {
        return;
    }

    FVector2D MousePosition;
    GetMousePosition(MousePosition.X, MousePosition.Y);

    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);

    CameraPawn->PanCamera(MousePosition, ViewportSizeX, ViewportSizeY, EdgePanMargin, PanSpeed, DeltaSeconds);
}

void ADragDropController::ZoomCamera(float DeltaSeconds)
{
    if (!CameraPawn)
    {
        return;
    }

    CameraPawn->ZoomCamera(TargetZoomAmount, DeltaSeconds, ZoomSmoothSpeed, MinZoomDistance, MaxZoomDistance, CurrentZoomAmount);
}

void ADragDropController::ZoomIn()
{
    TargetZoomAmount += ZoomSpeed;
}

void ADragDropController::ZoomOut()
{
    TargetZoomAmount -= ZoomSpeed;
}

void ADragDropController::SetActorPlacement(APlaceableMesh* PlaceableMesh)
{
    if (!PlaceableMesh)
    {
        if (bIsInPlacementMode && CurrentPlaceableMesh)
        {
            CurrentPlaceableMesh->Destroy();
        }
        bIsInPlacementMode = false;
        CurrentPlaceableMesh = nullptr;
        bShowMouseCursor = true;
    }
    else
    {
        if (bIsInPlacementMode && CurrentPlaceableMesh != PlaceableMesh)
        {
            CurrentPlaceableMesh->Destroy();
        }
        bIsInPlacementMode = true;
        CurrentPlaceableMesh = PlaceableMesh;
        bShowMouseCursor = false;
    }
}

void ADragDropController::CancelActorPlacement()
{
    if (CurrentPlaceableMesh)
    {
        CurrentPlaceableMesh->Destroy();
    }
    SetActorPlacement(nullptr);
}

FCollisionQueryParams ADragDropController::GetPlaceableTraceParams() const
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(CurrentPlaceableMesh);
    CollisionParams.AddIgnoredActor(CameraPawn);
    return CollisionParams;
}

void ADragDropController::ProjectPlaceableMesh()
{
    if (!CurrentPlaceableMesh || !bIsInPlacementMode)
    {
        return;
    }

    FVector2D MousePosition;
    GetMousePosition(MousePosition.X, MousePosition.Y);

    FVector WorldLocation;
    FVector WorldDirection;
    DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams = GetPlaceableTraceParams();

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.0f, ECollisionChannel::ECC_Visibility, CollisionParams);

    if (bHit)
    {
        FVector HitLocation = HitResult.Location;
        FVector HitNormal = HitResult.Normal;

        // Check if the hit surface normal is within the acceptable angle
        float DotProduct = FVector::DotProduct(HitNormal, FVector::UpVector);
        float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

        if (CurrentPlaceableMesh->CanPlaceAngle(Angle))
        {
            // Check for overlap at the intended placement location
            FCollisionShape CollisionShape = FCollisionShape::MakeBox(CurrentPlaceableMesh->GetComponentsBoundingBox().GetExtent());
            bool bIsOverlapping = GetWorld()->OverlapAnyTestByChannel(HitLocation, FQuat::Identity, ECC_Pawn, CollisionShape, CollisionParams);

            if (bIsOverlapping)
            {
                // Find the closest non-overlapping location
                FVector NewLocation = FindClosestNonOverlappingLocation(HitLocation, CollisionShape, CollisionParams);
                CurrentPlaceableMesh->SetActorLocation(NewLocation);
                //CurrentPlaceableMesh->LastPosition = NewLocation;
            }
            else
            {
                CurrentPlaceableMesh->SetActorLocation(HitLocation);
                //CurrentPlaceableMesh->LastPosition = HitLocation;
            }
            return;
        }
    }

    // If the placement is invalid, keep the mesh at its last valid position
    CurrentPlaceableMesh->SetActorLocation(CurrentPlaceableMesh->GetLastPosition());
}

FVector ADragDropController::FindClosestNonOverlappingLocation(const FVector& StartLocation, const FCollisionShape& CollisionShape, const FCollisionQueryParams& CollisionParams)
{
    const float StepSize = 10.0f; // Step size for each incremental move
    const int MaxIterations = 100; // Max number of iterations to avoid infinite loops
    FVector BestLocation = StartLocation;

    for (int i = 0; i < MaxIterations; ++i)
    {
        // Check if the current location is valid
        bool bIsOverlapping = GetWorld()->OverlapAnyTestByChannel(BestLocation, FQuat::Identity, ECC_Pawn, CollisionShape, CollisionParams);

        if (!bIsOverlapping)
        {
            return BestLocation;
        }

        // Move the location slightly in a random direction
        BestLocation += FVector(FMath::RandRange(-StepSize, StepSize), FMath::RandRange(-StepSize, StepSize), 0);
    }

    // If no valid location found, return the original location
    return StartLocation;
}


void ADragDropController::SelectPlaceableOne()
{
    if (PlaceableMeshes.Num() > 0)
    {
        TSubclassOf<APlaceableMesh> PlaceableMesh = PlaceableMeshes[0];
        APlaceableMesh* NewPlaceableMesh = GetWorld()->SpawnActor<APlaceableMesh>(PlaceableMesh, FVector::ZeroVector, FRotator::ZeroRotator);
        SetActorPlacement(NewPlaceableMesh);
    }
}

void ADragDropController::LeftClick()
{
    if (bIsInPlacementMode && CurrentPlaceableMesh)
    {
        CurrentPlaceableMesh = nullptr;
        SetActorPlacement(nullptr);
    }
}

void ADragDropController::RightClick()
{
    if (bIsInPlacementMode)
    {
        CancelActorPlacement();
    }
}
