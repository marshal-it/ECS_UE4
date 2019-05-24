// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "GroupObserver.h"
#include "Group.h"

UGroup::UGroup(const class FObjectInitializer& Obj)
	:Super(Obj)
{

}

UGroup::~UGroup()
{

}

void UGroup::Init(UMatcher* Matcher)
{
	this->mMatcher = Matcher;
}

auto UGroup::Count() const -> const int32
{
	return 	mEntities.Num();
}

auto UGroup::GetEntities() ->TArray<AEntity*>
{
	if (mEntitiesCache.Num() == 0 && mEntities.Num() > 0)
	{
		mEntitiesCache = TArray<AEntity*>(mEntities);
	}
	return mEntitiesCache;
}

auto UGroup::GetSingleEntity() const->AEntity*
{
	auto count = Count();
	if (count == 1)
	{
		return *(mEntities.GetData());
	}
	else if (count == 0)
	{
		return nullptr;
	}
	else
	{
		UE_LOG(ECS_LOG, Log, TEXT(" cannot get the single entity from group. Group contains more than one entity"));
	}
	return nullptr;
}

bool UGroup::ContainsEntity(const AEntity* entity) const
{
	return mEntities.Contains(entity);
}

UGroupObserver* UGroup::CreateObserver(const GroupEventType eventType)
{
	return NewObject<UGroupObserver>();
}

auto UGroup::GetMatcher()->UMatcher*
{
	return mMatcher;
}

void UGroup::SetInstance(UGroup* instance)
{
	mInstance = instance;
}

auto UGroup::HandleEntity(AEntity* entity) ->FGroupChanged*
{
	return  mMatcher->Matches(entity) ? AddEntity(entity) : RemoveEntity(entity);
}

void UGroup::HandleEntitySilently(AEntity* entity)
{
	if (mMatcher->Matches(entity))
	{
		AddEntitySilently(entity);
	}
	else
	{
		RemoveEntitySilently(entity);
	}
}

void UGroup::HandleEntity(AEntity* entity, ComponentId index, IComponent* component)
{
	if (mMatcher->Matches(entity))
	{
		AddEntity(entity, index, component);
	}
	else
	{
		RemoveEntity(entity, index, component);
	}

}

void UGroup::UpdateEntity(AEntity* entity, ComponentId index, IComponent* previousComponent, IComponent* newComponent)
{
	if (ContainsEntity(entity))
	{
		OnEntityRemoved.Broadcast(mInstance.Get(), entity, index, previousComponent);
		OnEntityAdded.Broadcast(mInstance.Get(), entity, index, newComponent);
		OnEntityUpdated.Broadcast(mInstance.Get(), entity, index, previousComponent, newComponent);
	}
}

void UGroup::RemoveAllEventHandlers()
{
	OnEntityAdded.Clear();
	OnEntityRemoved.Clear();
	OnEntityUpdated.Clear();
}

bool UGroup::AddEntitySilently(AEntity* entity)
{
	if (mEntities.AddUnique(entity))
	{
		mEntitiesCache.Empty();
		return true;
	}
	return false;
}

void UGroup::AddEntity(AEntity* entity, ComponentId index, IComponent* component)
{
	if (AddEntitySilently(entity))
	{
		OnEntityAdded.Broadcast(mInstance.Get(), entity, index, component);
	}
}

auto UGroup::AddEntity(AEntity* entity) ->FGroupChanged*
{
	return AddEntitySilently(entity) ? &OnEntityAdded : nullptr;
}

bool UGroup::RemoveEntitySilently(AEntity* entity)
{
	if (mEntities.Remove(entity))
	{
		mEntitiesCache.Empty();
		return true;
	}
	return false;
}

void UGroup::RemoveEntity(AEntity* entity, ComponentId index, IComponent* component)
{
	if (RemoveEntitySilently(entity))
	{
		OnEntityRemoved.Broadcast(mInstance.Get(), entity, index, component);
	}
}

auto UGroup::RemoveEntity(AEntity* entity) ->FGroupChanged*
{
	return RemoveEntitySilently(entity) ? &OnEntityRemoved : nullptr;
}

