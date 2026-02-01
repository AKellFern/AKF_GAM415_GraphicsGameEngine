// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

//Perlin Math was created to simulate the randomness of nature


// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Attachment setup 
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	procMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();
	
	//Create the mesh
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
	for (int i = 0; i < Vertices.Num(); i++)
	{
		FVector tempVector = impactPoint - this->GetActorLocation();

		if (FVector(Vertices[i] - tempVector).Size() < radius)
		{
			Vertices[i] = Vertices[i] - Depth;
			procMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}
}

void APerlinProcTerrain::CreateVertices()
{
	//Nested loop to create vertices based on the X and Y size
	for (int X = 0; X <= XSize; X++)
	{
		for (int Y = 0; Y <= YSize; Y++)
		{
			//Calculate the Z value using Perlin noise
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier;
			
			//Debugging to see the Z values
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));

			//Add the vertices to the array
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));

			//Add UVs
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));

		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	//Temporary variable to keep track of the current vertex
	int Vertex = 0;

	//Nested loop to create triangles based on the X and Y size
	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{

			//First triangle
			Triangles.Add(Vertex);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 1);
			
			//Second triangle
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);
			//Increment the vertex to move to the next column
			Vertex++;
		}
		//Increment the vertex to skip to the next row
		Vertex++;
	}
}

