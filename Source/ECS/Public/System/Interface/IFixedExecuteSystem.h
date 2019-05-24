// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ISystem.h"
#include "IFixedExecuteSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFixedExecuteSystem : public USystem
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API IFixedExecuteSystem : public ISystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FixedExecute() = 0;
	
};
