// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ComponentTypeId.h"
#include "DelegateImpl.h"
#include <stack>
#include <map>
#include "Entity.generated.h"

/*
 * 实体类
*/

DECLARE_MULTICAST_DELEGATE_ThreeParams(FEntityChanged, AEntity*, ComponentId, IComponent*);
DECLARE_MULTICAST_DELEGATE_FourParams(FComponentReplaced, AEntity*, ComponentId, IComponent*, IComponent*);
DECLARE_MULTICAST_DELEGATE_OneParam(FEntityReleased, AEntity*);


UCLASS()
class ECS_API AEntity : public AActor
{
	GENERATED_BODY()

public:	
	AEntity(const class FObjectInitializer& Obj);
	// Sets default values for this actor's properties

	virtual void Init(TMap<ComponentId, TArray<IComponent*>>* componentPools);

	template<typename T, typename... TArgs> inline auto Add(TSubclassOf<class UObject> Class = nullptr, TArgs&&... args)->AEntity*;
	template<typename T> inline auto Remove()->AEntity*;
	template <typename T, typename... TArgs> inline auto Replace(TArgs&&... args)->AEntity*;
	template <typename T> inline auto Refresh()->AEntity*;
	template <typename T> inline auto Get() const->T*;
	template <typename T> inline auto Use()->T*;
	template <typename T> inline bool Has() const;

	bool HasComponents(const std::vector<ComponentId>& indices) const;
	bool HasAnyComponent(const std::vector<ComponentId>& indices) const;
	auto GetComponentsCount() const -> unsigned int;
	void RemoveAllComponents();
	auto GetUuid() const -> const unsigned int;
	bool IsEnabled();

	bool operator == (const AEntity*& right) const;
	bool operator == (const AEntity right) const;

	FEntityChanged OnComponentAdded;
	FComponentReplaced OnComponentReplaced;
	FEntityChanged OnComponentRemoved;
	FEntityReleased OnEntityReleased;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	template<typename T>
	void RegisterComponentPool(TMap<ComponentId, TArray<IComponent*>>* componentPools, IComponent* Com);

	void SetInstance(AEntity* instance);
	auto AddComponent(const ComponentId index, IComponent* component)->AEntity*;
	auto RemoveComponent(const ComponentId index)->AEntity*;
	auto ReplaceComponent(const ComponentId index, IComponent* component)->AEntity*;
	auto GetComponent(const ComponentId index) const->IComponent*;
	bool HasComponent(const ComponentId index) const;
	void Destroy();

	template<typename T, typename... TArgs> inline auto CreateComponent(TSubclassOf<class UObject> Class = nullptr, TArgs&&... args)->IComponent*;

	unsigned int mUuid{ 0 };
	bool mIsEnabled = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	friend class UPool;
	
	auto GetComponentPool(const ComponentId index) const->TArray<IComponent*>*;
	void Replace(const ComponentId index, IComponent* replacement);

	TWeakObjectPtr<AEntity> mInstance;

	TMap<ComponentId, IComponent*> mComponents;

	TMap<ComponentId, TArray<IComponent*>>* mComponentPools;
};


template<typename T>
void AEntity::RegisterComponentPool(TMap<ComponentId, TArray<IComponent*>>* componentPools, IComponent* Com)
{
	//自带组件初始化
	AddComponent(FComponentTypeId::Get<T>(), Com);
	(static_cast<T*>(Com))->Reset("test", "register");
	//if ((*componentPools).Contains(id))
	//{
	//	(*componentPools)[id].AddUnique(Com);
	//}
	//else
	//{
	//	(*componentPools).Add(id, TArray<IComponent*>());
	//	(*componentPools)[id].AddUnique(Com);
	//}
}

template <typename T, typename... TArgs>
auto AEntity::CreateComponent(TSubclassOf<class UObject> Class, TArgs&&... args) -> IComponent*
{
	//从Pool的组件池中查找是否有可用的组件
	TArray<IComponent*>* componentPool = GetComponentPool(FComponentTypeId::Get<T>());
	IComponent* component = nullptr;

	if (componentPool != nullptr && componentPool->Num() > 0)
	{
		//取出Pool组件池中的组件，并从池中移除
		component = (*componentPool)[componentPool->Num() - 1];
		(*componentPool).Remove(component);
	}
	else
	{
		//Pool的组件池中并没有需要的组件，自己创建
		//考虑对接虚幻的组件，有两种基类类型（SceneComponent,ActorComponent）
//		UClass* test = Class.LoadSynchronous()->StaticClass();
		//
		//template< class T >
		//FUNCTION_NON_NULL_RETURN_START
		//	T* NewObject(UObject* Outer, FName Name, EObjectFlags Flags = RF_NoFlags, UObject* Template = nullptr, bool bCopyTransientsFromClassDefaults = false, FObjectInstancingGraph* InInstanceGraph = nullptr)
		//	FUNCTION_NON_NULL_RETURN_END

		component = NewObject<T>(this, Class);
	//	component = NewObject<T>(this, TEXT("Component"), EObjectFlags::RF_NeedLoad, Class.LoadSynchronous());
		check(component);
		if (Cast<UActorComponent>(component))
		{
			this->AddOwnedComponent(Cast<UActorComponent>(component));
			Cast<UActorComponent>(component)->RegisterComponent();
		}
		else if (Cast<USceneComponent>(component))
		{
			EAttachmentRule InRule = EAttachmentRule::SnapToTarget;
			FAttachmentTransformRules Rules(InRule, InRule, InRule, false);
			Cast<USceneComponent>(component)->AttachToComponent(this->GetRootComponent(), Rules);
			Cast<USceneComponent>(component)->RegisterComponent();
		}

		//Pool的组件池中没有这个组件，但是这里不会将entity中自身的component记录到Pool
		//componentPool->AddUnique(component);
	}
	(static_cast<T*>(component))->Reset(std::forward<TArgs>(args)...);

	return component;
}

template <typename T, typename... TArgs>
auto AEntity::Add(TSubclassOf<class UObject> Class, TArgs&&... args)->AEntity*
{
	return AddComponent(FComponentTypeId::Get<T>(), CreateComponent<T>(Class, std::forward<TArgs>(args)...));
}
template <typename T>
auto AEntity::Remove() -> AEntity*
{
	return RemoveComponent(FComponentTypeId::Get<T>());
}

template <typename T, typename... TArgs>
auto AEntity::Replace(TArgs&&... args) -> AEntity*
{
	return ReplaceComponent(FComponentTypeId::Get<T>(), CreateComponent<T>(std::forward<TArgs>(args)...));
}

template <typename T>
auto AEntity::Refresh() -> AEntity*
{
	return ReplaceComponent(FComponentTypeId::Get<T>(), Get<T>());
}

template<typename T>
auto AEntity::Get() const -> T*
{
	return static_cast<T*>(GetComponent(FComponentTypeId::Get<T>()));
}

template<typename T>
auto AEntity::Use() -> T*
{
	Refresh<T>();
	return static_cast<T*>(GetComponent(FComponentTypeId::Get<T>()));
}

template <typename T>
bool AEntity::Has() const
{
	return HasComponent(FComponentTypeId::Get<T>());
}