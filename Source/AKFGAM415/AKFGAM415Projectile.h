// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// Needed for Dynamic Material Instance
#include "Materials/MaterialInstanceDynamic.h"
#include "AKFGAM415Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class AKFGAM415_API AAKFGAM415Projectile : public AActor
{
	GENERATED_BODY()

public:
	AAKFGAM415Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Collision component for hit detection
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	// Controls projectile movement behavior
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;

	// Mesh used for projectile visuals
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh;

	// Base material used to create dynamic material instance
	UPROPERTY(EditAnywhere)
	UMaterialInterface* projMat;

	// Dynamic Material Instance used to change projectile color at runtime
	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;

	// Stores the randomized projectile color for decal matching
	FLinearColor randColor;

	// Base decal material for impact splat
	UPROPERTY(EditAnywhere)
	UMaterial* baseMat;

public:
	// Called when projectile hits another object
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
