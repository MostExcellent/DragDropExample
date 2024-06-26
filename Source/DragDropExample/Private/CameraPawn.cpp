#include "CameraPawn.h"

ACameraPawn::ACameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize Camera
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;
}

void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ACameraPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ACameraPawn::PanCamera(FVector2D MousePosition, int32 ViewportSizeX, int32 ViewportSizeY, float EdgePanMargin, float PanSpeed, float DeltaSeconds)
{
    FVector CameraMoveDirection = FVector::ZeroVector;

    if (MousePosition.X <= ViewportSizeX * EdgePanMargin / 100)
    {
        CameraMoveDirection.Y = -1.0f;
    }
    else if (MousePosition.X >= ViewportSizeX - (ViewportSizeX * EdgePanMargin / 100))
    {
        CameraMoveDirection.Y = 1.0f;
    }

    if (MousePosition.Y <= ViewportSizeY * EdgePanMargin / 100)
    {
        CameraMoveDirection.X = 1.0f;
    }
    else if (MousePosition.Y >= ViewportSizeY - (ViewportSizeY * EdgePanMargin / 100))
    {
        CameraMoveDirection.X = -1.0f;
    }

    FVector NewLocation = GetActorLocation() + (CameraMoveDirection * PanSpeed * DeltaSeconds);
    SetActorLocation(NewLocation);
}

void ACameraPawn::ZoomCamera(float TargetZoomAmount, float DeltaSeconds, float ZoomSmoothSpeed, float MinZoomDistance, float MaxZoomDistance, float& CurrentZoomAmount)
{
    if (FMath::IsNearlyZero(TargetZoomAmount))
    {
        return;
    }

    float DesiredZoom = FMath::FInterpTo(CurrentZoomAmount, TargetZoomAmount, DeltaSeconds, ZoomSmoothSpeed);
    float ZoomAmount = DesiredZoom - CurrentZoomAmount;

    CurrentZoomAmount = DesiredZoom;

    FVector NewLocation = GetActorLocation();
    NewLocation.Z = FMath::Clamp(NewLocation.Z + ZoomAmount, MinZoomDistance, MaxZoomDistance);
    SetActorLocation(NewLocation);
}
