// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcPlane.h"
#include "ProceduralMeshComponent.h"


// Sets default values
AProcPlane::AProcPlane()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the procedural mesh component
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Proc Mesh"));

	

}

// Called when the game starts or when spawned
void AProcPlane::BeginPlay()
{
	Super::BeginPlay();

}

//Creation before the game loads
//Called after the actor is created
//Best Practice is to call the parent first, added this in for habit building
void AProcPlane::PostActorCreated()
{
	Super::PostActorCreated();
	CreateMesh();
	//Set a UV material	
	if (PlaneMat)
	{
		procMesh->SetMaterial(0, PlaneMat);
	}
}

void AProcPlane::PostLoad()
{
	Super::PostLoad();
	CreateMesh();
}

// Called every frame
void AProcPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//Function to create the mesh
void AProcPlane::CreateMesh()
{
	procMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
}

