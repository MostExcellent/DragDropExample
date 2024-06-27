#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CameraPawn.h"
#include "InputAction.h"
#include "PlaceableMesh.h"
#include "DragDropController.generated.h"

class UInputMappingContext;

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
    void PanCamera(float DeltaSeconds) const;
    void ZoomCamera(float DeltaSeconds);
    void ZoomIn();
    void ZoomOut();

private:
    UPROPERTY(BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    ACameraPawn* CameraPawn;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* ZoomInAction;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* ZoomOutAction;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* LeftClickAction;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* RightClickAction;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* CancelPlacementAction;

    UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SelectPlaceableOneAction;

    UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float PanSpeed = 400.0f;

    UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float EdgePanMargin = 10.0f;

    UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float ZoomSpeed = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float MinZoomDistance = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float MaxZoomDistance = 3500.0f;

    UPROPERTY(EditAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float ZoomSmoothSpeed = 0.1f;

    float TargetZoomAmount = 0.0f;
    float CurrentZoomAmount = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Placement", meta = (AllowPrivateAccess = "true"))
    bool bIsInPlacementMode = false;

    UPROPERTY(BlueprintReadOnly, Category = "Placement", meta = (AllowPrivateAccess = "true"))
    APlaceableMesh* CurrentPlaceableMesh;

    UPROPERTY(EditAnywhere, Category = "Placement", meta = (AllowPrivateAccess = "true"))
    TArray<TSubclassOf<APlaceableMesh>> PlaceableMeshes;

public:
    UFUNCTION(BlueprintCallable, Category = "Placement")
    void SetActorPlacement(APlaceableMesh* PlaceableMesh);

    UFUNCTION(BlueprintCallable, Category = "Placement")
    void CancelActorPlacement();

private:
    FCollisionQueryParams GetPlaceableTraceParams() const;
    void ProjectPlaceableMesh() const;
    void SelectPlaceableOne();
    void LeftClick();
    void RightClick();
};
