// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "SystemContainer.h"
#include "Pool.h"
#include "EntityDemoSystem.h"
#include "ECSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ECS_API AECSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
};
