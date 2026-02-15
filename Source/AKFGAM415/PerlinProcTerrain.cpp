// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Perlin Math was created to simulate the randomness of nature

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
	// We do not need Tick for this terrain
	PrimaryActorTick.bCanEverTick = false;

	// Attachment setup for the procedural mesh component
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	procMesh->SetupAttachment(GetRootComponent());
}

// Used Construction method to view terrain in editor
void APerlinProcTerrain::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Clear previous mesh data so it does not stack in editor
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();
	Normals.Reset();
	UpVertexColors.Reset();

	// Rebuild terrain so changes to XSize/YSize update instantly
	CreateVertices();
	CreateTriangles();

	procMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);
	procMesh->SetMaterial(0, Mat);
}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();

	// Create the mesh at runtime as well
	CreateVertices();
	CreateTriangles();

	procMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);
	procMesh->SetMaterial(0, Mat);
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APerlinProcTerrain::AlterMesh(FVector impactPoint)
{
	// Loop through vertices and modify those within radius of impact
	for (int i = 0; i < Vertices.Num(); i++)
	{
		FVector tempVector = impactPoint - this->GetActorLocation();

		if (FVector(Vertices[i] - tempVector).Size() < radius)
		{
			// Lower vertex by Depth amount
			Vertices[i] = Vertices[i] - Depth;

			// Update the mesh section to reflect changes
			procMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}
}

void APerlinProcTerrain::CreateVertices()
{
	// Nested loop to create vertices based on the X and Y size
	for (int X = 0; X <= XSize; X++)
	{
		for (int Y = 0; Y <= YSize; Y++)
		{
			// Calculate the Z value using Perlin noise
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier;

			// Add the vertices to the array
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));

			// Add UVs for material mapping
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	// Temporary variable to keep track of the current vertex
	int Vertex = 0;

	// Nested loop to create triangles based on the X and Y size
	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{
			// First triangle
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);

			// Second triangle
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			// Move to next column
			Vertex++;
		}

		// Skip to next row
		Vertex++;
	}
}
