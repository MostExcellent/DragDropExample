#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CameraPawn.h"
#include "InputAction.h"
#include "PlaceableMesh.h"
#include "DragDropController.generated.h"

class UInputMappingContext;

/**
 * Player controller for placing actors in the world.
 */
UCLASS()
class DRAGDROPEXAMPLE_API ADragDropController : public APlayerController
{
    GENERATED_BODY()

public:
    ADragDropController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    void PanCamera(float DeltaSeconds);

	void ZoomCamera(float DeltaSeconds);
    void ZoomIn();
    void ZoomOut();

protected:

    // Camera pawn
	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	ACameraPawn* CameraPawn;

    // Input mapping context
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;

    // Zoom in and out actions
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ZoomInAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ZoomOutAction;

	// Left and right click actions
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LeftClickAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RightClickAction;

	// Cancel placement action
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CancelPlacementAction;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float PanSpeed;

    // Maybe make this be a percentage of viewport size?
    UPROPERTY(EditAnywhere, Category = "Camera")
    float EdgePanMargin;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float ZoomSpeed;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float MinZoomDistance;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float MaxZoomDistance;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float ZoomSmoothSpeed;

    float TargetZoomAmount;
    float CurrentZoomAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Placement")
    bool bIsInPlacementMode;

	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	APlaceableMesh* CurrentPlaceableMesh;

public:

    UFUNCTION(BlueprintCallable, Category = "Placement")
    void SetActorPlacement(APlaceableMesh* PlaceableMesh);

    UFUNCTION(BlueprintCallable, Category = "Placement")
	void CancelActorPlacement();
};
