// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Pool.h"
#include "ISetPoolSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USetPoolSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API ISetPoolSystem
{
	GENERATED_BODY()

public:
	virtual void SetPool(UPool* pool) = 0;
};
