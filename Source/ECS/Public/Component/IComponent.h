// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ISystem.h"
#include "IComponent.generated.h"

/*
 * 实体组件
*/

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UComponent : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */

class ECS_API IComponent
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	friend class AEntity;
};
