// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ISystem.h"
#include "ITickInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTickInterface : public USystem
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API ITickInterface : public ISystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool ExecuteTick(float deltaSecond) = 0;
};
