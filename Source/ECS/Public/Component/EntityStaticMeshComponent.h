// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "IComponent.h"
#include "EntityStaticMeshComponent.generated.h"

/**
 * 静态网格组件类
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ECS_API UEntityStaticMeshComponent : public UStaticMeshComponent, public IComponent
{
	GENERATED_BODY()
	
	
	
	
};
