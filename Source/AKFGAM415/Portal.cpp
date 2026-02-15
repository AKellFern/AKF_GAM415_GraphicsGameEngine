#include "Portal.h"
#include "AKFGAM415Character.h"
#include "Kismet/GameplayStatics.h"

// Constructs the portal actor and creates the core components used for this module:
// - Box trigger for overlap teleport
// - Mesh that displays the portal material (which samples the render target)
// - SceneCaptureComponent2D that renders the linked portal view into the render target
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create components for the portal system (trigger, visible surface, and capture camera)
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalBox"));
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalSceneCapture"));
	rootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Root Arrow"));

	// Root is the trigger volume so overlap events drive teleport functionality
	RootComponent = boxComp;

	// Mesh is attached to the trigger; this surface will display the portal material
	mesh->SetupAttachment(boxComp);

	// Scene capture is attached so we can position/rotate it relative to the portal surface
	sceneCapture->SetupAttachment(mesh);

	// Attach the arrow to the root to indicate the forward spawn direction for teleporting players
	rootArrow->SetupAttachment(RootComponent);

	// Create glow ring mesh (visual only)
	glowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlowMesh"));
	glowMesh->SetupAttachment(RootComponent);

	// This is just a floating visual, so no collision needed
	glowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	glowMesh->SetGenerateOverlapEvents(false);

	// The portal surface should not physically block anything; it is a visual surface.
	mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap to trigger teleport when the player enters the portal volume
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);

	// Prevent the portal mesh from appearing inside its own scene capture (avoids recursive “portal within portal” artifacts)
	// NOTE: If SetHiddenInSceneCapture fails to compile in your UE version, use: mesh->bHiddenInSceneCapture = true;
	mesh->SetHiddenInSceneCapture(true);

	// Apply the portal material to the portal mesh.
	// The material is expected to sample a TextureRenderTarget2D that receives the live feed from the SceneCaptureComponent2D.
	if (mat)
	{
		mesh->SetMaterial(0, mat);
	}

	// Create Dynamic Material Instance for the glow ring so we can change it at runtime
	if (glowMat)
	{
		glowMID = UMaterialInstanceDynamic::Create(glowMat, this);
		glowMesh->SetMaterial(0, glowMID);

		// Example MID parameters (these must match parameter names in your glow material)
		glowMID->SetVectorParameterValue("GlowColor", FLinearColor(0.2f, 0.6f, 1.0f, 1.0f));
		glowMID->SetScalarParameterValue("GlowIntensity", 12.0f);
	}

	// Apply per-portal glow values
	glowMID->SetVectorParameterValue("GlowColor", glowColor);
	glowMID->SetScalarParameterValue("GlowIntensity", glowBaseIntensity);

}

void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Updates the capture camera transform each frame to simulate depth through the portal.
	// This supports the module requirement to calculate distance + rotation using scene capture + render targets.
	// Prevents use if no other portal is linked (avoids nullptr crashes).
	if (otherPortal && sceneCapture)
	{
		UpdatePortals();
	}

	if (glowMID)
	{
		float pulse = (FMath::Sin(GetWorld()->TimeSeconds * 2.0f) + 1.0f) * 0.5f; // 0..1
		glowMID->SetScalarParameterValue("GlowIntensity", glowBaseIntensity + (pulse * 6.0f));
	}

}

void APortal::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only teleport the player character (prevents random physics actors from triggering portal travel)
	AAKFGAM415Character* playerChar = Cast<AAKFGAM415Character>(OtherActor);

	// Teleport requires a linked destination portal
	if (playerChar && otherPortal)
	{
		// Prevent immediate re-triggering / ping-pong teleporting while overlapping the portal volumes
		if (!playerChar->isTeleporting)
		{
			playerChar->isTeleporting = true;

			// Move the player to the destination portal location
			FVector loc = otherPortal->rootArrow->GetComponentLocation();
			FRotator rot = otherPortal->rootArrow->GetForwardVector().Rotation();
			playerChar->SetActorLocation(loc);

			// Rotate the player to face forward relative to the destination portal orientation
			playerChar->SetActorRotation(rot);

			if (AController* Controller = playerChar->GetController())
			{
				Controller->SetControlRotation(rot);
			}

			// Timer resets the teleport lockout after a short delay so the player can safely exit the destination trigger
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, "SetBool", playerChar);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);
		}
	}
}

void APortal::SetBool(AAKFGAM415Character* playerChar)
{
	// Re-enables teleporting after the cooldown delay.
	// This avoids rapid overlap triggering and stabilizes portal traversal behavior.
	if (playerChar)
	{
		playerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	// Updates the scene capture transform so the render target shows a convincing “through the portal” view.
	// We offset the capture camera by the relative portal-to-portal displacement while matching the player camera rotation.
	// This is the core technique for simulating depth using SceneCaptureComponent2D + RenderTarget (module focus).
	// Prevents use if no other portal is linked (avoids nullptr crashes).
	if (!otherPortal || !sceneCapture)
	{
		return;
	}

	// Relative offset between this portal and the linked portal
	FVector Location = this->GetActorLocation() - otherPortal->GetActorLocation();

	// Player camera transform (what the player is currently seeing)
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();

	// Move the capture camera so it mirrors the player camera, but on the linked portal side
	FVector combinedLocation = Location + camLocation;

	// Apply the updated transform to the scene capture so the render target updates live
	sceneCapture->SetWorldLocationAndRotation(combinedLocation, camRotation);
}
