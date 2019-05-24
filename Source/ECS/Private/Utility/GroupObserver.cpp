// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "GroupObserver.h"

UGroupObserver::UGroupObserver(const class FObjectInitializer& Obj)
{

}

void UGroupObserver::Init(UGroup* group, const GroupEventType eventType)
{
	mGroups.AddUnique(group);
	mEventTypes.AddUnique(eventType);
	GroupAddEntityCacheHandle.CreateUObject(this, &UGroupObserver::AddEntity);
}

void UGroupObserver::Init(TArray<UGroup*> groups, TArray<GroupEventType> eventTypes)
{
	mGroups = groups;
	mEventTypes = eventTypes;
	GroupAddEntityCacheHandle.CreateUObject(this, &UGroupObserver::AddEntity);
	if (groups.Num() != eventTypes.Num())
	{
		UE_LOG(ECS_LOG, Log, TEXT("group and eventType vector counts must be equal"));
		return;
	}
	Activate();
}

void UGroupObserver::Activate()
{
	for (unsigned int i = 0, groupCount = mGroups.Num(); i < groupCount; ++i)
	{
		auto g = mGroups[i];
		auto eventType = mEventTypes[i];
		if (eventType == GroupEventType::OnEntityAdded)
		{
			g->OnEntityAdded.Remove(GroupAddEntityCacheHandle.GetHandle());
			g->OnEntityAdded.Add(GroupAddEntityCacheHandle);
		}
		else if (eventType == GroupEventType::OnEntityRemoved)
		{
			g->OnEntityRemoved.Remove(GroupAddEntityCacheHandle.GetHandle());
			g->OnEntityRemoved.Add(GroupAddEntityCacheHandle);
		}
		else if (eventType == GroupEventType::OnEntityAddedOrRemoved)
		{
			g->OnEntityAdded.Remove(GroupAddEntityCacheHandle.GetHandle());
			g->OnEntityAdded.Add(GroupAddEntityCacheHandle);

			g->OnEntityRemoved.Remove(GroupAddEntityCacheHandle.GetHandle());
			g->OnEntityRemoved.Add(GroupAddEntityCacheHandle);
		}
	}
}

void UGroupObserver::Deactivate()
{
	//for (const auto &g : mGroups)
	//{
	//	if (g != nullptr)
	//	{
	//		g->OnEntityAdded.Remove(GroupAddEntityCacheHandle.GetHandle());
	//		g->OnEntityRemoved.Remove(GroupAddEntityCacheHandle.GetHandle());
	//	}
	//}

	ClearCollectedEntities();
}

auto UGroupObserver::GetCollectionEntities() ->TArray<AEntity*>
{
	return mCollectionEntities;
}

void UGroupObserver::ClearCollectedEntities()
{
	mCollectionEntities.Empty();
}

void UGroupObserver::AddEntity(UGroup* group, AEntity* entity, ComponentId index, IComponent* component)
{
	mCollectionEntities.AddUnique(entity);
}
