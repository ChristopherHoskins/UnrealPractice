// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

  owner = GetOwner();
  if (!owner)
    return;
  if (!pressurePlate)
  {
    UE_LOG(LogTemp, Error, TEXT("Missing pressure plate on %s"), *GetOwner()->GetName())
  }
  originalYaw = owner->GetActorRotation().Yaw;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // Poll the trigger volume
  // If the actorThatOpens is in the volume
  if (GetTotalMassOfActorsOnPlate() > triggerMass)
  {
    onOpenRequest.Broadcast();
  }
  else
  {
    onCloseRequest.Broadcast();
  }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
  float totalMass = 0.0f;

  // Find all overlapping actors
  TArray<AActor*> overlappingActors;

  if (!pressurePlate) { return totalMass; }
  pressurePlate->GetOverlappingActors(OUT overlappingActors);
  
  // Iterate through them adding their masses
  for (const auto& actor : overlappingActors)
  {
    totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
  }

  return totalMass;
}
