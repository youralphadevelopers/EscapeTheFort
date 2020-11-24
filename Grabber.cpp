// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h" 


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	findPhysicsHandle();
	setupInputCommands();

	// ...
	
}

void UGrabber::setupInputCommands(){

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent){
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::findPhysicsHandle(){

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab(){

	// UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(

		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Draw a ViewPortLine
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	//If We hit something then attatch to pawn
	if(ActorHit){
		if (!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation
					(
					ComponentToGrab,
					NAME_None,
					GetPlayersReach()
					);
	}
}

void UGrabber::Release(){
	
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	// If the physic handle is attach.
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
	// ...
}

// Return the first Actor within reach with physics body.

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(

		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Draw a ViewPortLine

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult Hit;

	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//AActor* ActorHit = Hit.GetActor();
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(

		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// Draw a ViewPortLine

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
