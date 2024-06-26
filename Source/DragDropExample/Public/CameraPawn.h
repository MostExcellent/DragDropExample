#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "CameraPawn.generated.h"

UCLASS()
class DRAGDROPEXAMPLE_API ACameraPawn : public APawn
{
    GENERATED_BODY()

public:
    ACameraPawn();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

public:
    void PanCamera(FVector2D MousePosition, int32 ViewportSizeX, int32 ViewportSizeY, float EdgePanMargin, float PanSpeed, float DeltaSeconds);
    void ZoomCamera(float TargetZoomAmount, float DeltaSeconds, float ZoomSmoothSpeed, float MinZoomDistance, float MaxZoomDistance, float& CurrentZoomAmount);

protected:
    UPROPERTY(EditAnywhere, Category = "Camera")
    UCameraComponent* CameraComponent;
};