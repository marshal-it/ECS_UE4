// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "Entity.h"
#include "ISystem.h"
#include "ISetPoolSystem.h"
#include "Pool.h"
#include "IReactiveSystem.h"
#include "SystemReactive.h"

UPool::UPool(const class FObjectInitializer& Obj)
	:Super(Obj)
{

}

void UPool::Init(const unsigned int startCreationIndex /*= 1*/)
{
	mCreationIndex = startCreationIndex;
	OnEntityReleasedCache.CreateUObject(this, &UPool::OnEntityReleased);
}

UPool::~UPool()
{
	mGroups.Empty();
	mGroupsForIndex.Empty();
	mEntities.Empty();
	mReusableEntities.Empty();
	mRetainedEntities.Empty();
	mEntitiesCache.Empty();
	ClearComponentPools();
}

void UPool::InitWorldPtr(UWorld* world)
{
	if (world == nullptr)
	{
		UE_LOG(ECS_LOG, Log, TEXT("UWorld is nullptr, pool init failed!"));
	}
	mWorld = world;
}

void UPool::RegisterEntity(AEntity* entity)
{
	if (entity != nullptr )
	{
		entity->Init(&mComponentPools);
		entity->OnEntityReleased.Broadcast(entity);
		entity->SetInstance(entity);
		entity->mIsEnabled = true;
		entity->mUuid = mCreationIndex++;
		mEntities.AddUnique(entity);
		mEntitiesCache.Empty();

		entity->OnComponentAdded.AddUObject(this, &UPool::UpdateGroupsComponentAddedOrRemoved);
		entity->OnComponentRemoved.AddUObject(this, &UPool::UpdateGroupsComponentAddedOrRemoved);
		entity->OnComponentReplaced.AddUObject(this, &UPool::UpdateGroupsComponentReplaced);

		entity->OnEntityReleased.Clear();
		entity->OnEntityReleased.Add(OnEntityReleasedCache);

		//通知实体创建的消息
		OnEntityCreated.Broadcast(this, entity);
	}
}

bool UPool::HasEntity(const AEntity* entity) const
{
	return mEntities.Contains(entity);
}

void UPool::DestroyEntity(AEntity* entity)
{
	int32 removedIndex = mEntities.Remove(entity);
	if (removedIndex < 0 )
	{
		UE_LOG(ECS_LOG, Log, TEXT("Destroyed faild !"));
	}
	//清空缓存
	mEntitiesCache.Empty();
	//广播删除操作
	OnEntityWillBeDestroyed.Broadcast(this, entity);
	//执行组件数据删除
	entity->Destroy();
	//广播实体销毁
	OnEntityDestroyed.Broadcast(this, entity);

	//实体是否可用
	if (entity->IsValidLowLevel())
	{
		//清除恢复实体的代理
		entity->OnEntityReleased.Remove(OnEntityReleasedCache.GetHandle());
		//加入到可重用实体池
		mReusableEntities.AddUnique(entity);
	}
	//暂不用保留池
	//else
	//{
	//	mRetainedEntities.AddUnique(entity);
	//}
}

void UPool::DestroyAllEntities()
{
	//拷贝一份实体
	auto entitiesTemp = TArray<AEntity*>(mEntities);
	int32 eNum = entitiesTemp.Num();
	while (eNum > 0)
	{
		//清除拷贝实体池
		eNum--;
		DestroyEntity(entitiesTemp[eNum]);
		entitiesTemp.RemoveAt(eNum);
	}
	//清空实体池
	mEntities.Empty();

	//判断保留池是否还有实体，如果还有，则异常（目前还没用上）
	if (mRetainedEntities.Num() > 0)
	{
		UE_LOG(ECS_LOG, Log, TEXT(" pool detected retained entities although all entities got destroyed. Did you release all entities?"));
	}
}

auto UPool::GetEntities() ->TArray<AEntity*>
{
	if (mEntitiesCache.Num() <= 0)
	{
		//如果缓存实体里面没有实体，则重新拷贝一份
		mEntitiesCache = TArray<AEntity*>(mEntities);
	}
	return mEntitiesCache;
}

auto UPool::GetEntities(UMatcher* Matcher)->TArray<AEntity*>
{
	return GetGroup(Matcher)->GetEntities();
} 

auto UPool::GetGroup(UMatcher* Matcher) ->UGroup*
{
	UGroup* group = nullptr;
	if (mGroups.Contains(Matcher))
	{
		return mGroups[Matcher];
	}
	else
	{
		group = NewObject<UGroup>();
		group->Init(Matcher);
		group->SetInstance(group);

		auto entities = GetEntities();
		for (int32 i =0, entitiesLength = entities.Num(); i < entitiesLength; i++)
		{
			group->HandleEntitySilently(entities[i]);
		}
		mGroups.Add(group->GetMatcher(), group);
		for (int i =0, indicesLength = Matcher->GetIndices().size(); i < indicesLength; i++)
		{
			if (mGroupsForIndex.Contains(Matcher->GetIndices()[i]))
			{
				mGroupsForIndex[Matcher->GetIndices()[i]].AddUnique(group);
			}
			else
			{
				TArray<TWeakObjectPtr<UGroup>> WeakGroupObj;
				WeakGroupObj.AddUnique(group);
				mGroupsForIndex.Add(Matcher->GetIndices()[i], WeakGroupObj);
			}
		}
		OnGroupCreated.Broadcast(this, group);
	}
	return group;
}

void UPool::ClearGroups()
{
	for (TMap<UMatcher*, UGroup*>::TIterator It(mGroups); It; ++It)
	{
		It.Value()->RemoveAllEventHandlers();
		OnGroupCleared.Broadcast(this, It.Value());
	}

	mGroups.Empty();

	for (TMap<ComponentId, TArray<TWeakObjectPtr<UGroup>>>::TIterator It(mGroupsForIndex); It; ++It)
	{
		It.Value().Empty();
	}
	mGroupsForIndex.Empty();
}

void UPool::ResetCreationIndex()
{
	mCreationIndex = 0;
}

void UPool::ClearComponentPool(const ComponentId index)
{
	mComponentPools[index].Empty();
	mComponentPools.Remove(index);
}

void UPool::ClearComponentPools()
{
	TArray<ComponentId> Keys;
	mComponentPools.GetKeys(Keys);
	for (int32 i=0; i < Keys.Num(); i++)
	{
		ClearComponentPool(Keys[i]);
	}
}

void UPool::Reset()
{
	ClearGroups();
	DestroyAllEntities();
	ResetCreationIndex();
}

auto UPool::GetEntityCount() const ->int32
{
	return mEntities.Num();
}

auto UPool::GetReusableEntitiesCount() const ->int32
{
	return mReusableEntities.Num();
}

auto UPool::GetRetainedEntitiesCount() const ->int32
{
	return mRetainedEntities.Num();
}

auto UPool::CreateSystem(ISystem* system) ->ISystem*
{
	if (Cast<ISetPoolSystem>(system) != nullptr)
	{
		Cast<ISetPoolSystem>(system)->SetPool(this);
	}

	if (Cast<IReactiveSystem>(system) != nullptr)
	{
		USystemReactive* tempSystem = NewObject<USystemReactive>();
		tempSystem->Init(this, Cast<IReactiveSystem>(tempSystem));
		return  Cast<ISystem>(tempSystem);
	}
	return system;
}

UWorld* UPool::GetPoolWorld()
{
	return mWorld;
}

void UPool::UpdateGroupsComponentAddedOrRemoved(AEntity* entity, ComponentId index, IComponent* component)
{
	if (!mGroupsForIndex.Contains(index))
	{
		return;
	}

	auto groups = mGroupsForIndex[index];
	if (groups.Num() > 0)
	{
		auto events = TArray<FGroupChanged*>();
		for (int i = 0, groupsCount = groups.Num(); i < groupsCount; ++i)
		{
			events.AddUnique(groups[i].Get()->HandleEntity(entity));
		}

		for (int i = 0, eventsCount= events.Num(); i < eventsCount; ++i)
		{
			(*events[i]).Broadcast(groups[i].Get(), entity, index, component);
		}
	}
}

void UPool::UpdateGroupsComponentReplaced(AEntity* entity, ComponentId index, IComponent* previousComponent, IComponent* newComponent)
{
	if (!mGroupsForIndex.Contains(index))
	{
		return;
	}
	if (mGroupsForIndex[index].Num() > 0)
	{
		for (const auto &g:mGroupsForIndex[index])
		{
			g.Get()->UpdateEntity(entity, index, previousComponent, newComponent);
		}
	}
}

void UPool::OnEntityReleased(AEntity* entity)
{
	if (entity->mIsEnabled)
	{
		UE_LOG(ECS_LOG, Log, TEXT("Error, cannot release entity. Entity is not destroyed yet"));
	}

	mRetainedEntities.Remove(entity);
	mReusableEntities.Remove(entity);
}
