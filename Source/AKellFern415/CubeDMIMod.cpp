// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "AKellFern415Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and initialize the collision box and visual mesh components
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));

	// Set the Box Component as the root and attach the mesh so it inherits transforms
	RootComponent = BoxComponent;
	CubeMesh->SetupAttachment(BoxComponent);



}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();

	// When the game starts, call the On Overlap Function when something overlaps the Box Component
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

	//Check if the base material is valid and then create and assign a dynamic material instance
	if (BaseMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	}

	// Check if the Cube Mesh is valid and if so then assign the created DMI to this Cube Mesh
	if (CubeMesh)
	{
		CubeMesh->SetMaterial(0, DynamicMaterial);
	}



	
}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIMod::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAKellFern415Character* overlappedActor = Cast<AAKellFern415Character>(OtherActor);

	if (overlappedActor)
	{
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranDarkness = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		FVector4 randColor = FVector4(ranNumX, ranNumY, ranNumZ, 1.f);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("Color", randColor);
			DynamicMaterial->SetScalarParameterValue("Darkness", ranDarkness);

		}
	}

}

