// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "EntityMovementComponent.h"

UEntityMovementComponent::UEntityMovementComponent(const class FObjectInitializer& Obj)
	:Super(Obj)
{

}

void UEntityMovementComponent::Reset(const std::string& name1, const std::string& name2)
{

}

void UEntityMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	MoveUpdatedComponent(Velocity, FRotator::ZeroRotator, false);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(ECS_LOG, Log, TEXT("EntityMovementComponent ---%s----!!!"), *Velocity.ToString());
}
