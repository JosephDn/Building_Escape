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
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Do Nothing
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input component found for %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this /*points to class executing*/, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this /*points to class executing*/, &UGrabber::Release);		
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed successfully"));
	GetFirstPhysicsBodyInReach();
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber released successfully"));
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line trace has hit %s"), *(ActorHit->GetName()));
	}

	return Hit;
}

// Draws debug line
void UGrabber::DebugLine()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5
	);
}
