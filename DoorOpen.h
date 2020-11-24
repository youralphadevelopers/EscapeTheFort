// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorOpen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPETHEFORT_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

	float TotalMassOfActors() const;

	void FindAudioComponent();
	void FindpressurePlate();

	// Tracks whether the sound has been played.
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:

	float initialYaw;
	float currentYaw;

	UPROPERTY(EditAnywhere)
	float targetYaw = 250.f;

	UPROPERTY(EditAnywhere)
 	ATriggerVolume* TriggerLoad;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;
	 
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
 	AActor* ActorThatOpens;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

};
