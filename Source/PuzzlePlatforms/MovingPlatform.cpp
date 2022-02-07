// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "TimerManager.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}


void AMovingPlatform::BeginPlay() 
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		SetReplicates(true);
		SetReplicateMovement(true);

		TravelTime = TargetLocationHandle.Size() / PlatformSpeed;

		StartLocation = GetActorLocation();
		EndLocation = StartLocation + TargetLocationHandle;

		SimpleTimer = FTimerHandle();
		GetWorldTimerManager().SetTimer(SimpleTimer, this, &AMovingPlatform::EndReached, TravelTime + StopTime, true);
	}

	/*CanEverTick = true;

	bool bTest = CanEverTick();

	*/
}

void AMovingPlatform::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);
	
	if (HasAuthority())
	{
		float PercentToTargetLocation = (GetWorldTimerManager().GetTimerElapsed(SimpleTimer) - StopTime) / TravelTime;

		PercentToTargetLocation = FMath::Clamp(PercentToTargetLocation, 0.f, 1.f);

		FVector Loc = FMath::Lerp(StartLocation, EndLocation, PercentToTargetLocation);

		SetActorLocation(Loc);
	}
}

void AMovingPlatform::EndReached() 
{
	FVector Temp;
	Temp = StartLocation;
	StartLocation = EndLocation;
	EndLocation = Temp;

	//SimpleTimer = FTimerHandle();
	//GetWorldTimerManager().SetTimer(SimpleTimer, this, &AMovingPlatform::EndReached, TravelTime, false, StopTime);
}