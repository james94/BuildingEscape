// Copyright James Medel 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 200.f; // 1 meter reach

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	// Return the first Actor within reach with a physics body.
	// const since we don't want it changing what it ends up getting hit by
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Return The Line Trace End
	FVector GetPlayersReach() const;

	// Get Players Position In the World
	FVector GetPlayersWorldPos() const;
};
