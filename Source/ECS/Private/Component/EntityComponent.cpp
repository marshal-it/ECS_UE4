// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "EntityComponent.h"


// Sets default values for this component's properties
UEntityComponent::UEntityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEntityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEntityComponent::Reset(const std::string& name1, const std::string& name2)
{
	FString cstring(name1.c_str());
	UE_LOG(ECS_LOG, Log, TEXT(" Entity Component is reset!--%s"), *cstring);
}

