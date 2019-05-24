#include "ECS.h"
#include "DemoEntity.h"

ADemoEntity::ADemoEntity(const class FObjectInitializer& Obj)
	:Super(Obj)
{
	EntityRoot = CreateDefaultSubobject<UEntityRootComponent>(TEXT("EntityRootCom"));
	RootComponent = EntityRoot;
}

void ADemoEntity::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADemoEntity::Init(TMap<ComponentId, TArray<IComponent*>>* componentPools)
{
	Super::Init(componentPools);
	RegisterComponentPool<UEntityRootComponent>(componentPools, EntityRoot);
}