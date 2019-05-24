// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ISystem.h"
#include "Entity.h"
#include "IReactiveExecuteSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UReactiveExecuteSystem : public USystem
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API IReactiveExecuteSystem : public ISystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Execute(TArray<AEntity*> entities) = 0;
	
};
