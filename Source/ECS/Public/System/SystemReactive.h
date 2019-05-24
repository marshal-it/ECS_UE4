// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "IMultiReactiveSystem.h"
#include "IReactiveExecuteSystem.h"
#include "IReactiveSystem.h"
#include "IEnsureComponents.h"
#include "IExcludeComponents.h"
#include "IClearReactiveSystem.h"
#include "IExecuteSystem.h"
#include "Entity.h"
#include "TriggerOnEvent.h"
#include "Matcher.h"
#include "GroupObserver.h"
#include "Pool.h"
#include "SystemReactive.generated.h"

/**
 * 被动执行系统
 */
UCLASS()
class ECS_API USystemReactive : public UObject, public IExecuteSystem
{
	GENERATED_BODY()
	
public:
	USystemReactive(const class FObjectInitializer& Obj);

	//初始化子系统数据
	void Init(UPool* pool, IReactiveSystem* subSystem);
	void Init(UPool* pool, IMultiReactiveSystem* subSystem);
	void Init(UPool* pool, IReactiveExecuteSystem* subSystem, const TArray<FTriggerOnEvent> triggers);
	~USystemReactive();

	auto GetSubSystem() const->IReactiveExecuteSystem*;
	void Activate();
	void Deactivate();
	void Clear();

	void Execute() override;

private:
	IReactiveExecuteSystem* mSubSystem;
	UPROPERTY()
	UGroupObserver* mObserver;
	UPROPERTY()
	UMatcher* mEnsureComponents;
	UPROPERTY()
	UMatcher* mExecuteComponents;
	UPROPERTY()
	bool ClearAfterExecute = false;
	UPROPERTY()
	TArray<AEntity*> mEntityBuffer;
};
