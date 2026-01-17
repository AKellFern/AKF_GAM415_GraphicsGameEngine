// Copyright Epic Games, Inc. All Rights Reserved.

#include "AKFGAM415PickUpComponent.h"

UAKFGAM415PickUpComponent::UAKFGAM415PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UAKFGAM415PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UAKFGAM415PickUpComponent::OnSphereBeginOverlap);
}

void UAKFGAM415PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AAKFGAM415Character* Character = Cast<AAKFGAM415Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
