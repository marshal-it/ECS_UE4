// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <vector>
#include "IComponent.h"
#include "ComponentTypeId.generated.h"

/**
 * 组件类型ID
 */

typedef unsigned int ComponentId;
typedef std::vector<ComponentId> ComponentIdList;

USTRUCT()
struct FComponentTypeId
{
	GENERATED_USTRUCT_BODY()

public:
	FComponentTypeId() {};
	template<typename T>
	static const ComponentId Get()
	{
		static_assert((std::is_base_of<IComponent, T>::value && !std::is_same<IComponent, T>::value),
			"Class type must be derived from IComponent");
		static ComponentId id = mCounter++;
		return id;
	}

	static unsigned int Count()
	{
		return mCounter;
	}

private:
	static unsigned int mCounter;
};
