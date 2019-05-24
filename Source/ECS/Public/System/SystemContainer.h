// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include <vector>
#include "InitializeSystem.h"
#include "ITickInterface.h"
#include "IExecuteSystem.h"
#include "IFixedExecuteSystem.h"
#include "SystemContainer.generated.h"

/**
 * 系统收集器
 */
UCLASS()
class ECS_API USystemContainer : public UObject, public ITickInterface, public IInitializeSystem, public IExecuteSystem, public IFixedExecuteSystem
{
	GENERATED_BODY()
	
public:
	USystemContainer(const class FObjectInitializer& Obj);
	~USystemContainer();
	USystemContainer* Add(ISystem* system);
	template<typename T> 	inline USystemContainer* Add();

	void Initaialize(UWorld* _world)  override;
	void Execute() override;
	void FixedExecute() override;

	void ActivateReactiveSystems();
	void DeactivateReactiveSystems();
	void ClearReactiveSystems();

	bool ExecuteTick(float deltaSecond) override;

private:
	TArray<IInitializeSystem*> mInitializeSystems;
	TArray<IExecuteSystem*> mExecuteSystems;
	TArray<IFixedExecuteSystem*> mFixedExecuteSystems;
	TArray<ITickInterface*> mTickExecuteSystems;

	FTickerDelegate TickDelegate;
	FDelegateHandle TickDelegateHandle;
};

template<typename T>
USystemContainer* USystemContainer::Add()
{
	return Add(NewObject<T>());
}