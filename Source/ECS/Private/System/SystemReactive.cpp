// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "SystemReactive.h"

USystemReactive::USystemReactive(const class FObjectInitializer& Obj)
	:Super(Obj)
{

}

void USystemReactive::Init(UPool* pool, IReactiveSystem* subSystem)
{
	TArray<FTriggerOnEvent> Events;
	Events.Add(subSystem->trigger);
	Init(pool, subSystem, Events);
}

void USystemReactive::Init(UPool* pool, IMultiReactiveSystem* subSystem)
{
	Init(pool, subSystem, subSystem->triggers);
}

void USystemReactive::Init(UPool* pool, IReactiveExecuteSystem* subSystem, const TArray<FTriggerOnEvent> triggers)
{
	mSubSystem = subSystem;
	//初始化子系统数据
	if (Cast<IEnsureComponents>(subSystem) != nullptr)
	{
		mEnsureComponents = Cast<IEnsureComponents>(subSystem)->ensureComponents;
	}

	if (Cast<IExcludeComponents>(subSystem) != nullptr)
	{
		mEnsureComponents = Cast<IExcludeComponents>(subSystem)->executeComponents;
	}

	if (Cast<IClearReactiveSystem>(subSystem) != nullptr)
	{
		ClearAfterExecute = true;
	}
	int32 triggerLength = triggers.Num();
	TArray<UGroup*> groups;
	TArray<GroupEventType> eventTypes;

	for (int32 i =0; i < triggerLength; i++)
	{
		FTriggerOnEvent trigger = triggers[i];
		groups.AddUnique(pool->GetGroup(trigger.trigger));
		eventTypes.AddUnique(trigger.eventType);
	}

	mObserver = NewObject<UGroupObserver>();
	mObserver->Init(groups, eventTypes);
}

USystemReactive::~USystemReactive()
{
	//这里不需要管理释放，有自动的GC管理
	//	Deactivate();
	//	mObserver->ConditionalBeginDestroy();
	mSubSystem = nullptr;
}

auto USystemReactive::GetSubSystem() const->IReactiveExecuteSystem*
{
	return mSubSystem;
}

void USystemReactive::Activate()
{
	mObserver->Activate();
}

void USystemReactive::Deactivate()
{
	mObserver->Deactivate();
}

void USystemReactive::Clear()
{
	mObserver->ClearCollectedEntities();
}

void USystemReactive::Execute()
{
	if (mObserver->GetCollectionEntities().Num() != 0)
	{
		if (!mEnsureComponents->IsEmpty())
		{
			if (!mExecuteComponents->IsEmpty())
			{
				for (const auto e : mObserver->GetCollectionEntities())
				{
					if (mEnsureComponents->Matches(e) && mExecuteComponents->Matches(e))
					{
						mEntityBuffer.AddUnique(e);
					}
				}
			}
			else
			{
				for (const auto e : mObserver->GetCollectionEntities())
				{
					if (mEnsureComponents->Matches(e))
					{
						mEntityBuffer.AddUnique(e);
					}
				}
			}
		}
		else if (!mExecuteComponents->IsEmpty())
		{
			for (const auto e : mObserver->GetCollectionEntities())
			{
				if (!mExecuteComponents->Matches(e))
				{
					mEntityBuffer.AddUnique(e);
				}
			}
		}
		else
		{
			for (const auto e : mObserver->GetCollectionEntities())
			{
				mEntityBuffer.AddUnique(e);
			}
		}

		mObserver->ClearCollectedEntities();
		if (mEntityBuffer.Num() != 0)
		{
			mSubSystem->Execute(mEntityBuffer);
			mEntityBuffer.Empty();
			if (ClearAfterExecute)
			{
				mObserver->ClearCollectedEntities();
			}
		}
	}
}
