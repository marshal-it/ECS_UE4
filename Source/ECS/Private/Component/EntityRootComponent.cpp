// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "EntityRootComponent.h"


// Sets default values for this component's properties
UEntityRootComponent::UEntityRootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEntityRootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEntityRootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEntityRootComponent::Reset(const std::string& name1, const std::string& name2)
{
	UE_LOG(ECS_LOG, Log, TEXT(" Entity Root Component is reset!"));
}

