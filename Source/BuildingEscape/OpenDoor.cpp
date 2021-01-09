// Copyright James Medel 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	// Our TargetYaw will be our InitialYaw plus whatever is the value assigned to TargetYaw in our OpenDoor.h after
	// UPROPERTY(EditAnywhere). NOTE: we can even edit the TargetYaw for one or both of our doors in Unreal Engine
	// Editor and that will affect how much our one or both of our doors open.
	TargetYaw += InitialYaw; // TargetYaw = TargetYaw + InitialYaw

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set."), *GetOwner()->GetName());
	}

	// Initialize ActorThatOpens with Default Pawn. So for instance, when we press play, we'll see ActorThatOpens is already set
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If actor that opens is overlapping with our door trigger volume pressure plate, then open door
	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds(); // whatever time is in the world
	}
	else
	{
		// if the door has been open longer than DoorCloseDelay x seconds
		// in other words, if current time minus DoorLastOpened is greater than DoorCloseDelay, close door
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float &DeltaTime)
{
	// CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 0.5f);
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 72);// Open Door closer to 1 sec; 72 deg per sec
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float &DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, 90);// Slam to Close Door in 1 sec; 90 deg per sec
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}
