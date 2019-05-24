// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "IComponent.h"
#include <string>
#include "EntitySkeletalMeshComponent.generated.h"

/**
 * 骨骼组件
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ECS_API UEntitySkeletalMeshComponent : public USkeletalMeshComponent, public IComponent
{
	GENERATED_BODY()
	
public:
	void 	Reset(const std::string& name1, const std::string& name2);
	
};
