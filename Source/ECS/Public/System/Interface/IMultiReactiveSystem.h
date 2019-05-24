// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "IReactiveExecuteSystem.h"
#include <vector>
#include "TriggerOnEvent.h"
#include "IMultiReactiveSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMultiReactiveSystem : public UReactiveExecuteSystem
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API IMultiReactiveSystem : public IReactiveExecuteSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	TArray<FTriggerOnEvent> triggers;
	
};
