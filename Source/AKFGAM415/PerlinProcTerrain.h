// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcTerrain.generated.h"

// Forward declarations
class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class AKFGAM415_API APerlinProcTerrain : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APerlinProcTerrain();

	// Variables
	// Clamp the terrain size
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 0;

	// Z axis height multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.0f))
	float ZMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.0f))
	float NoiseScale = 1.0f;

	// Scale of the terrain spacing
	UPROPERTY(EditAnywhere, Meta = (ClampMin = .000001f))
	float Scale = 0.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = .000001f))
	float UVScale = 0.0f;

	// Width and Height of the terrain chunks
	UPROPERTY(EditAnywhere)
	float radius;

	UPROPERTY(EditAnywhere)
	FVector Depth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Used to rebuild terrain in editor when values change
	virtual void OnConstruction(const FTransform& Transform) override;

	// Setting the material for the terrain
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Mat;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to alter the terrain using raytrace impact
	UFUNCTION()
	void AlterMesh(FVector impactPoint);

private:
	// Procedural mesh component that holds the generated terrain
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* procMesh;

	// Mesh data arrays
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FColor> UpVertexColors;

	int sectionID = 0;

	// Functions that generate mesh data
	void CreateVertices();
	void CreateTriangles();
};
