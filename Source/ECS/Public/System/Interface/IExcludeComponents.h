// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Matcher.h"
#include "IExcludeComponents.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExcludeComponents : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECS_API IExcludeComponents
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UMatcher* executeComponents;
	
};
