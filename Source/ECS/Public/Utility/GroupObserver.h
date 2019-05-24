// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Group.h"
#include "Entity.h"
#include "GroupEventType.h"
#include "GroupObserver.generated.h"

/**
 * 实体组观察者
 */

class UGroup;

DECLARE_DELEGATE_FourParams(FGroupAddEntityCache, UGroup*, AEntity*, ComponentId, IComponent*);

UCLASS()
class  UGroupObserver : public UObject
{
	GENERATED_BODY()
	
public:
	UGroupObserver(const class FObjectInitializer& Obj);
	void Init(UGroup* group, const GroupEventType eventType);
	void Init(TArray<UGroup*> groups, TArray<GroupEventType> eventTypes);
	
	void Activate();
	void Deactivate();
	auto GetCollectionEntities()->TArray<AEntity*>;
	void ClearCollectedEntities();

private:
	void AddEntity(UGroup* group, AEntity* entity, ComponentId index, IComponent* component);

	UPROPERTY()
	TArray<AEntity*> mCollectionEntities;
	UPROPERTY()
	TArray<UGroup*> mGroups;
	TArray<GroupEventType> mEventTypes;

	FGroupAddEntityCache GroupAddEntityCacheHandle;
};
