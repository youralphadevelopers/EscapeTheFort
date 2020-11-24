// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorOpen.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();

	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	targetYaw += initialYaw; 

	FindpressurePlate();
	FindAudioComponent();
	// ...
}


// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	if(TotalMassOfActors() > MassToOpenDoors){
		OpenDoor(DeltaTime);
	}
	else{
		CloseDoor(DeltaTime);
	}
	
	// ...
}

void UDoorOpen::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());	
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
	}
}

void UDoorOpen::FindpressurePlate()
{
	if(!PressurePlate)
		{
			UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set!"), *GetOwner()->GetName());	
		}
}

void UDoorOpen::OpenDoor(float DeltaTime)
{
	currentYaw = FMath::Lerp(currentYaw, targetYaw, DeltaTime * 1.f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioComponent) {return;}
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UDoorOpen::CloseDoor(float DeltaTime)
{
	currentYaw = FMath::Lerp(currentYaw, initialYaw, DeltaTime * 2.f);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound = false;
	if (!AudioComponent) {return;}
	if (!CloseDoorSound) 
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}
}

float UDoorOpen:: TotalMassOfActors() const
{
	float TotalMass = 0.f;
	
	// Find all overlapping Actors :
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Add up their masses : 
	for(AActor* Actor : OverlappingActors){
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressureplate!"), *Actor->GetName());
	}

	return TotalMass;
}

