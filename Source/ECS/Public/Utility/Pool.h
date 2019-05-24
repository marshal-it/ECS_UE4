// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Matcher.h"
#include <unordered_map>
#include <unordered_set>
#include <map>
#include "Entity.h"
#include "Group.h"
#include "Pool.generated.h"

/**
 * 实体管理池
 */

DECLARE_DELEGATE_OneParam(FOnEntityReleasedCacheDel, AEntity*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FPoolChanged, UPool*, AEntity*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FGroupHandle, UPool*, UGroup*);


UCLASS()
class ECS_API UPool : public UObject
{
	GENERATED_BODY()
	
public:
	UPool(const class FObjectInitializer& Obj);
	void Init(const unsigned int startCreationIndex = 1);
	~UPool();

	//初始化世界指针
	void InitWorldPtr(UWorld* world);
	//创建实体
	template<typename T>
	auto CreateEntity(TSubclassOf<AEntity> SubClass = nullptr) -> AEntity*;
	//注册实体
	void RegisterEntity(AEntity* entity);
	//查找实体是否存在池中
	bool HasEntity(const AEntity* entity) const;
	void DestroyEntity(AEntity* entity); 
	void DestroyAllEntities();

	auto GetEntities()->TArray<AEntity*>;
	auto GetEntities(UMatcher* Matcher) ->TArray<AEntity*>;

	auto GetGroup(UMatcher* Matcher)->UGroup*;
	 
	void ClearGroups();
	void ResetCreationIndex();
	void ClearComponentPool(const ComponentId index);
	void ClearComponentPools();
	void Reset();

	auto GetEntityCount() const->int32;
	auto GetReusableEntitiesCount() const->int32;
	auto GetRetainedEntitiesCount() const->int32;

	auto CreateSystem(ISystem* system)->ISystem*;
	template<typename T> inline auto CreateSystem()->ISystem*;

	FPoolChanged OnEntityCreated; //实体创建消息代理
	FPoolChanged OnEntityWillBeDestroyed;//实体将要销毁消息代理
	FPoolChanged OnEntityDestroyed; //实体销毁消息代理
	FGroupHandle OnGroupCreated; //组创建消息代理
	FGroupHandle OnGroupCleared; //组清除

	//生成实体
	template<typename T>
	auto SpawnAEntity()->T*;

	//生成实体
	template<typename T>
	auto SpawnAEntity(TSubclassOf<AEntity> Entity)->T*;

	//拷贝实体生成
	template<typename T>
	auto SpawnAEntity(AEntity* Entity)->T*;
	UWorld* GetPoolWorld();

private:
	void UpdateGroupsComponentAddedOrRemoved(AEntity* entity, ComponentId index, IComponent* component);
	void UpdateGroupsComponentReplaced(AEntity* entity, ComponentId index, IComponent* previousComponent, IComponent* newComponent);
	void OnEntityReleased(AEntity* entity);

private:
	//恢复实体的代理
	FOnEntityReleasedCacheDel OnEntityReleasedCache;
	unsigned int mCreationIndex;
	//实体
	UPROPERTY()
	TArray<AEntity*> mEntities;
	//匹配筛选器-实体组
	UPROPERTY()
	TMap<UMatcher*, UGroup*> mGroups;
	//可重用实体池
	UPROPERTY()
	TArray<AEntity*> mReusableEntities;
	//保留池--
	UPROPERTY()
	TArray<AEntity*> mRetainedEntities;
	//组件池
	TMap<ComponentId, TArray<IComponent*>> mComponentPools;
	//组件-组
	TMap<ComponentId, TArray<TWeakObjectPtr<UGroup>>> mGroupsForIndex;
	//缓存实体
	UPROPERTY()
	TArray<AEntity*> mEntitiesCache;
	//世界上下文
	UPROPERTY()
	UWorld* mWorld = nullptr;
};

template<typename T>
inline auto UPool::CreateEntity(TSubclassOf<AEntity> SubClass) -> AEntity *
{
		T* entity = nullptr;
		int32 eNum = mReusableEntities.Num();
		if (eNum > 0)
		{
			//这里考虑到可重用类型和实际上需要实例的类型并不是同一个而仅仅是父子类型关系
			if (T::StaticClass() == mReusableEntities[eNum - 1]->GetClass())
			{
				entity = SpawnAEntity<T>(mReusableEntities[eNum - 1]);
				mReusableEntities.RemoveAt(eNum - 1);
			}
		}
		else
		{
			if (SubClass != nullptr)
			{
				entity = SpawnAEntity<T>(SubClass);
			}
			else
			{
				entity = SpawnAEntity<T>();
			}
			entity->OnEntityReleased.Broadcast(entity);
		}
		entity->mIsEnabled = true;
		entity->SetInstance(entity);
		entity->Init(&mComponentPools);
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
	
		return Cast<AEntity>(entity);
}

template <typename T>
auto UPool::CreateSystem() ->ISystem*
{
	return CreateSystem(Cast<ISystem>(NewObject<T>()));
}

template<typename T>
auto UPool::SpawnAEntity() ->T*
{
	return mWorld->SpawnActor<T>();
}
template<typename T>
auto UPool::SpawnAEntity(AEntity* Entity) ->T*
{
	FActorSpawnParameters Parameters;
	Parameters.Template = Entity;
	return mWorld->SpawnActor<T>(AEntity::StaticClass(), Parameters);
}

template<typename T>
auto UPool::SpawnAEntity(TSubclassOf<AEntity> Entity) ->T*
{
	return mWorld->SpawnActor<T>(Entity);
}