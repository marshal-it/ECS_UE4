// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/MovementComponent.h"
#include "IComponent.h"
#include <string>
#include "EntityMovementComponent.generated.h"

/**
 * 移动组件
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ECS_API UEntityMovementComponent : public UMovementComponent, public IComponent
{
	GENERATED_UCLASS_BODY()
public:
	void 	Reset(const std::string& name1, const std::string& name2);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
