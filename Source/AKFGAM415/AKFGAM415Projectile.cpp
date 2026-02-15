// Fill out your copyright notice in the Description page of Project Settings.

#include "AKFGAM415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
// Used for random color and frame selection
#include "Kismet/KismetMathLibrary.h"
// Niagara systems
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
// Perlin Terrain Proc
#include "PerlinProcTerrain.h"

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

	// Generate random color once so projectile, particle, and decal all match
	randColor = FLinearColor(
		UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f),
		UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f),
		UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f),
		1.0f
	);

	// Create Dynamic Material Instance for projectile
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
	ballMesh->SetMaterial(0, dmiMat);

	// Apply random color to projectile material
	dmiMat->SetVectorParameterValue("ProjColor", randColor);
}

void AAKFGAM415Projectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// Only add impulse and destroy projectile if we hit a physics simulating object
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(
			GetVelocity() * 100.0f,
			GetActorLocation()
		);

		Destroy();
	}

	if (OtherActor != nullptr)
	{
		// Spawn Niagara particle and apply matching color
		if (colorP)
		{
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				colorP,
				HitComp,
				NAME_None,
				FVector(0.0f),
				FRotator(0.0f),
				EAttachLocation::KeepRelativeOffset,
				true
			);

			if (particleComp)
			{
				particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);
			}

			ballMesh->DestroyComponent();
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}

		float frameNum = UKismetMathLibrary::RandomIntegerInRange(0, 3);

		// If we hit the procedural terrain, alter it
		APerlinProcTerrain* procTerrain = Cast<APerlinProcTerrain>(OtherActor);
		if (procTerrain)
		{
			procTerrain->AlterMesh(Hit.ImpactPoint);
		}

		// Spawn decal at impact point (simple version)
		FVector DecalSize = FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f));

		auto Decal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			baseMat,
			DecalSize,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			0.f
		);

		if (Decal)
		{
			auto MatInstance = Decal->CreateDynamicMaterialInstance();
			if (MatInstance)
			{
				MatInstance->SetVectorParameterValue("Color", randColor);
				MatInstance->SetScalarParameterValue("Frame", frameNum);
			}
		}
	}
}
