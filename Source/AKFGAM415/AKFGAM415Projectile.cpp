// Fill out your copyright notice in the Description page of Project Settings.

#include "AKFGAM415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
// Used for random color and frame selection
#include "Kismet/KismetMathLibrary.h"

AAKFGAM415Projectile::AAKFGAM415Projectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AAKFGAM415Projectile::OnHit);
	RootComponent = CollisionComp;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	ballMesh->SetupAttachment(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AAKFGAM415Projectile::BeginPlay()
{
	Super::BeginPlay();

	if (projMat)
	{
		// Create Dynamic Material Instance so projectile color can be changed at runtime
		dmiMat = ballMesh->CreateDynamicMaterialInstance(0, projMat);
	}

	// Generate and store a random color for this projectile instance
	randColor = FLinearColor::MakeRandomColor();

	if (dmiMat)
	{
		// Apply randomized color to the projectile material
		dmiMat->SetVectorParameterValue("ProjColor", randColor);
	}
}

void AAKFGAM415Projectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if (baseMat)
	{
		// Spawn decal at hit location
		UDecalComponent* decalComp = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			baseMat,
			FVector(10.0f),
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			5.0f
		);

		if (decalComp)
		{
			// Create Dynamic Material Instance for decal
			UMaterialInstanceDynamic* decalDMI = decalComp->CreateDynamicMaterialInstance();

			if (decalDMI)
			{
				// Randomize splat frame for visual variation
				float frame = UKismetMathLibrary::RandomFloatInRange(0.0f, 3.0f);
				decalDMI->SetScalarParameterValue("Frame", frame);

				// Match decal color to projectile color
				decalDMI->SetVectorParameterValue("Color", randColor);
			}
		}
	}

	Destroy();
}
