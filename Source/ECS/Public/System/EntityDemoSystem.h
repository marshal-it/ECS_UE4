// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "InitializeSystem.h"
#include "IExecuteSystem.h"
#include "ISetPoolSystem.h"
#include "ITickInterface.h"
#include "EntityDemoSystem.generated.h"

/**
*
*/
UCLASS()
class ECS_API UEntityDemoSystem : public UObject, public IInitializeSystem, public IExecuteSystem, public ISetPoolSystem, public ITickInterface
{
	GENERATED_BODY()

public:
	void Initaialize(UWorld* _world)  override;
	void Execute() override;
    void SetPool(UPool* pool) override;
	bool ExecuteTick(float deltaSecond) override;

private:
	UPROPERTY()
	UPool* mPool;
};
