#include "ECS.h"
#include "EntityRootComponent.h"
#include "EntityComponent.h"
#include "EntitySkeletalMeshComponent.h"
#include "EntityMovementComponent.h"
#include "SystemContainer.h"
#include "DemoEntity.h"
#include "LoadTableSystem.h"
#include "EntityDemoSystem.h"

void UEntityDemoSystem::Initaialize(UWorld* _world)
{
	TSubclassOf<AEntity> entityclass;

	if (FDataTableHelper::Get()->GetTableSystem().IsValid())
	{
		TArray<FEntityItemSceneTable*> Table = FDataTableHelper::Get()->GetTableSystem()->GetTableArray<FEntityItemSceneTable, ETableTypeId::SCENEITEM_TABLE>();
		
		//获取移动组件类
		FEntityComponentTable* ComponentClass = FDataTableHelper::Get()->GetTableSystem()->GetTableStruct<FEntityComponentTable, ETableTypeId::ENTITYCOMPONET_TABLE>("405");

		//使用方式
		//TableBase* ComponentTest = DataMap[ETableTypeId::ENTITYCLASS_TABLE];
		//TableClass<FString, FEntityClassAssetTable>* Component2 = static_cast<TableClass<FString, FEntityClassAssetTable>*>(ComponentTest);
		//check(Component2);
		//TMap<FString, FEntityClassAssetTable*> test = Component2->GetTableMap();

		for (int32 i = 0; i < Table.Num(); i++)
		{			
			entityclass = FDataTableHelper::Get()->GetTableSystem()->GetTableStruct<FEntityClassAssetTable, ETableTypeId::ENTITYCLASS_TABLE>(Table[i]->EntityClassId)->EntityClass;
			mPool->CreateEntity<AEntity>(entityclass)->Add<UEntityMovementComponent>(ComponentClass->ComponentClass, "foo", "bar")->SetActorTransform(Table[i]->ItemTransform);
		}
	}
	UE_LOG(ECS_LOG, Log, TEXT("EntityDemoSystem Initalize"));
}

void UEntityDemoSystem::Execute()
{ 
	//mPool->CreateEntity<ADemoEntity>()->Add<UEntityComponent>(nullptr, "foo", "bar");
	//int32 entitiesCount = mPool->GetGroup(Matcher_AllOf(UEntityComponent))->Count();
	UE_LOG(ECS_LOG, Log, TEXT("EntityDemoSystem Execute"));
}

void UEntityDemoSystem::SetPool(UPool* pool)
{
	this->mPool = pool;
}

bool UEntityDemoSystem::ExecuteTick(float deltaSecond)
{
	UE_LOG(ECS_LOG, Log, TEXT("EntityDemoSystem Execute Tick"));
	return true;
}
