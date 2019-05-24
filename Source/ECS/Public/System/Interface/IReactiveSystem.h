// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IReactiveExecuteSystem.h"
#include "TriggerOnEvent.h"
#include "IReactiveSystem.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UReactiveSystem : public UReactiveExecuteSystem
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API IReactiveSystem : public IReactiveExecuteSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FTriggerOnEvent trigger;
	
};
