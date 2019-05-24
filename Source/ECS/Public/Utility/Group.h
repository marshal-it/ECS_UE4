// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "GroupEventType.h"
#include "Matcher.h"
#include "Group.generated.h"

/**
 * 实体组
 */

class UGroup;
class UGroupObserver;

DECLARE_MULTICAST_DELEGATE_FourParams(FGroupChanged, UGroup*, AEntity*, ComponentId, IComponent*);
DECLARE_MULTICAST_DELEGATE_FiveParams(FGroupUpdated, UGroup*, AEntity*, ComponentId, IComponent*, IComponent*);


UCLASS()
class ECS_API UGroup : public UObject
{
	GENERATED_BODY()
	
public:
	UGroup(const class FObjectInitializer& Obj);
	~UGroup();

	void Init(UMatcher* Matcher);

	auto Count() const -> const int32;
	auto GetEntities()->TArray<AEntity*>;
	auto GetSingleEntity() const->AEntity*;
	bool ContainsEntity(const AEntity* entity) const;
	auto GetMatcher() ->UMatcher*;
	UGroupObserver* CreateObserver(const GroupEventType eventType);

	FGroupChanged OnEntityAdded;
	FGroupUpdated OnEntityUpdated;
	FGroupChanged OnEntityRemoved;

protected:
	void SetInstance(UGroup* instance);
	auto HandleEntity(AEntity* entity)->FGroupChanged*;
	void HandleEntitySilently(AEntity* entity);
	void HandleEntity(AEntity* entity, ComponentId index, IComponent* component);
	void UpdateEntity(AEntity* entity, ComponentId index, IComponent* previousComponent, IComponent* newComponent);
	void RemoveAllEventHandlers();

private:
	bool AddEntitySilently(AEntity* entity);
	void AddEntity(AEntity* entity, ComponentId index, IComponent* component);
	auto AddEntity(AEntity* entity)->FGroupChanged*;
	bool RemoveEntitySilently(AEntity* entity);
	void RemoveEntity(AEntity* entity, ComponentId index, IComponent* component);
	auto RemoveEntity(AEntity* entity)->FGroupChanged*;

	TWeakObjectPtr<UGroup> mInstance;
	UPROPERTY()
	UMatcher* mMatcher;
	UPROPERTY()
	TArray<AEntity*> mEntities;
	UPROPERTY()
	TArray<AEntity*> mEntitiesCache;

private:
	friend class UPool;
};
