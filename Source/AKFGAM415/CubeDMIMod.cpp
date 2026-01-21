// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeDMIMod.h"
#include "AKFGAM415Character.h"
// Used for generating random values
#include "Kismet/KismetMathLibrary.h"
//Niagara systems	
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));

	RootComponent = boxComp;
	cubeMesh->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap event
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

	if (baseMat)
	{
		// Create Dynamic Material Instance so material parameters can be changed at runtime
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);
	}

	if (cubeMesh)
	{
		// Apply the dynamic material instance to the mesh
		cubeMesh->SetMaterial(0, dmiMat);
	}
}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeDMIMod::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	AAKFGAM415Character* overlappedActor = Cast<AAKFGAM415Character>(OtherActor);

	if (overlappedActor)
	{
		// Generate random color values
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

		// Store random color for the material parameter
		FLinearColor randColor = FLinearColor(ranNumX, ranNumY, ranNumZ, 1.f);

		if (dmiMat)
		{
			// Apply color and scalar parameters to the material
			dmiMat->SetVectorParameterValue("Color", randColor);
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);


			if (dmiMat)
			{
				UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
					colorP,
					OtherComp,
					NAME_None,
					FVector(0.f),
					FRotator(0.f),
					EAttachLocation::KeepRelativeOffset,
					true
				);

				particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);
			}
		}
	}
}
