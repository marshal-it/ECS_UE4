/**
@ 
@Author TeenWolf
@Email  731796991@qq.com
@Date   2017-08-22 11:11:44
*/


#pragma once
#include "Entity.h"
#include "EntityRootComponent.h"
#include "DemoEntity.generated.h"

UCLASS()
class ECS_API ADemoEntity : public AEntity
{
   GENERATED_BODY()
public:
	ADemoEntity(const class FObjectInitializer& Obj);

	virtual void PostInitializeComponents() override;

	virtual void Init(TMap<ComponentId, TArray<IComponent*>>* componentPools) override;

	UPROPERTY()
	UEntityRootComponent* EntityRoot;
private:

};