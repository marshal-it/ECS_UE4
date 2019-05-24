// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "Entity.h"

AEntity::AEntity(const class FObjectInitializer& Obj)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntity::Init(TMap<ComponentId, TArray<IComponent*>>* componentPools)
{
	//先注册自身自带的组件交给Pool去记录
	mComponentPools = componentPools;
}

void AEntity::SetInstance(AEntity* instance)
{
	mInstance = instance;
}

auto AEntity::AddComponent(const ComponentId index, IComponent* component) ->AEntity*
{
	if (!mIsEnabled)
	{
		UE_LOG(ECS_LOG, Log, TEXT("AEntity is not Enabled, maybe have destroyed"));
		return nullptr;
	}
	if (HasComponent(index))
	{
		UE_LOG(ECS_LOG, Log, TEXT("AEntity is contain same index component"));
		return nullptr;
	}
	mComponents.Add(index, component);
	check(mInstance.Get());
	OnComponentAdded.Broadcast(mInstance.Get(), index, component);

	return mInstance.Get();
}

auto AEntity::RemoveComponent(const ComponentId index) ->AEntity*
{
	if (!mIsEnabled)
	{
		UE_LOG(ECS_LOG, Log, TEXT("AEntity is not Enabled,  mybe have destroyed"));
		return nullptr;
	}
	if (!HasComponent(index))
	{
		UE_LOG(ECS_LOG, Log, TEXT("AEntity is not contain index component"));
		return nullptr;
	}

	Replace(index, nullptr);
	return mInstance.Get();
}

auto AEntity::ReplaceComponent(const ComponentId index, IComponent* component) ->AEntity*
{
	if (!mIsEnabled)
	{
		UE_LOG(ECS_LOG, Log, TEXT("AEntity is not Enabled,  mybe have destroyed"));
	}

	if (HasComponent(index))
	{
		Replace(index, component);
	}
	else if (component != nullptr)
	{
		AddComponent(index, component);
	}
	return mInstance.Get();
}

auto AEntity::GetComponent(const ComponentId index) const->IComponent*
{
	if (!HasComponent(index))
	{
		UE_LOG(ECS_LOG, Log, TEXT("AEntity is not contain index component"));
	}
	return mComponents[index];
}

bool AEntity::HasComponent(const ComponentId index) const
{
	return mComponents.Contains(index);
}

void AEntity::Destroy()
{
	RemoveAllComponents();
	OnComponentAdded.Clear();
	OnComponentReplaced.Clear();
	OnComponentRemoved.Clear();
	mIsEnabled = false;
}

bool AEntity::HasComponents(const std::vector<ComponentId>& indices) const
{
	for (const ComponentId &index : indices)
	{
		if (!HasComponent(index))
		{
			return false;
		}
	}
	return true;
}

bool AEntity::HasAnyComponent(const std::vector<ComponentId>& indices) const
{
	for (const ComponentId &index : indices)
	{
		if (HasComponent(index))
		{
			return true;
		}
	}
	return false;
}

auto AEntity::GetComponentsCount() const -> unsigned int
{
	return mComponents.Num();
}

void AEntity::RemoveAllComponents()
{
	auto componentsIdTemp = std::vector<ComponentId>(mComponents.Num());
	for (TMap<ComponentId, IComponent*>::TIterator It(mComponents);It;++It)
	{
		componentsIdTemp.push_back(It.Key());
	}

	while (componentsIdTemp.size() != 0)
	{
		Replace(componentsIdTemp.back(), nullptr);
		componentsIdTemp.pop_back();
	}

	mComponents.Empty();
}

auto AEntity::GetUuid() const -> const unsigned int
{
	return mUuid;
}

bool AEntity::IsEnabled()
{
	return mIsEnabled;
}

bool AEntity::operator ==(const AEntity*& right) const
{
	return this->GetUuid() == right->GetUuid();
}

bool AEntity::operator ==(const AEntity right) const
{
	return this->GetUuid() == right.GetUuid();
}

auto AEntity::GetComponentPool(const ComponentId index) const->TArray<IComponent*>*
{
	if (!(*mComponentPools).Contains(index))
	{
		(*mComponentPools).Add(index, TArray<IComponent*>());
	}
	return &((*mComponentPools)[index]);
}

void AEntity::Replace(const ComponentId index, IComponent* replacement)
{
	auto previousComponent = GetComponent(index);
	if (previousComponent == replacement)
	{
		OnComponentReplaced.Broadcast(mInstance.Get(), index, previousComponent, replacement);
	}
	else
	{
		GetComponentPool(index)->AddUnique(previousComponent);
		if (replacement == nullptr)
		{
			mComponents.Remove(index);
			OnComponentRemoved.Broadcast(mInstance.Get(), index, previousComponent);
		}
		else
		{
			mComponents[index] = replacement;
			OnComponentReplaced.Broadcast(mInstance.Get(), index, previousComponent, replacement);
		}
	}
}

// Called when the game starts or when spawned
void AEntity::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
