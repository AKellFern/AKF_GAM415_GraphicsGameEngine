// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// SceneCaptureComponent2D is a camera-like component that renders the scene from a viewpoint.
// In this module, it is used to capture what the linked portal looks like and output that view to a render target.
#include "Components/SceneCaptureComponent2D.h"

// TextureRenderTarget2D is a GPU-backed texture that can receive a live render from a scene capture.
// The portal material reads from this render target to display a real-time view of the destination portal.
#include "Engine/TextureRenderTarget2D.h"

// BoxComponent is used as a trigger volume to detect player overlap for portal teleportation.
#include "Components/BoxComponent.h"

// Include the player character header to reference the player class in overlap handling and state management.
#include "AKFGAM415Character.h"

// ArrowComponent to aid in the Player Spawn from the portal , providing a visual indicator of the spawn orientation.
#include "Components/ArrowComponent.h"

#include "Portal.generated.h"

UCLASS()
class AKFGAM415_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Portal surface mesh (the material applied here shows the render target live feed).
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	// Captures the scene from the destination portal viewpoint and renders into the render target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* sceneCapture;

	// Arrow component to indicate the spawn orientation for the player when teleporting through the portal.
	UPROPERTY(EditAnywhere)
	UArrowComponent* rootArrow;

	// The texture that receives the scene capture output; used by the portal material to display the destination.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* renderTarget;

	// Trigger volume that detects player overlap for teleporting through the portal.
	UPROPERTY(EditAnywhere)
	UBoxComponent* boxComp;

	// Linked portal reference (set in editor). This defines the destination for both the teleport and the captured view.
	UPROPERTY(EditAnywhere)
	APortal* otherPortal;

	// Material applied to the portal mesh that samples the renderTarget to display a live portal view.
	UPROPERTY(EditAnywhere)
	UMaterialInterface* mat;

	// Overlap handler: when the player enters the portal trigger, teleport them to the linked portal
	// and orient them to face forward relative to the destination portal.
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Controls overlap/teleport state to prevent repeat triggers or immediate re-teleporting after arrival.
	// (This supports stable portal traversal behavior while the portal view continues rendering via the capture/render target.)
	UFUNCTION()
	void SetBool(AAKFGAM415Character* playerChar);

	// Updates the portal rendering logic each frame.
	// Repositions and rotates the SceneCaptureComponent2D relative to the linked portal
	// so the render target displays a live, depth-correct view of the destination.
	UFUNCTION()
	void UpdatePortals();

};
