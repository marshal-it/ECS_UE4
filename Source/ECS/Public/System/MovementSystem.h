/**
@ 控制游戏关于移动的系统
@Author TeenWolf
@Email  731796991@qq.com
@Date   2017-09-09 10:29:08
*/


#pragma once
#include "InitializeSystem.h"
#include "IExecuteSystem.h"
#include "ITickInterface.h"
#include "ISetPoolSystem.h"
#include "MovementSystem.generated.h"

UCLASS()
class ECS_API UMovementSystem : public UObject, public IInitializeSystem, public IExecuteSystem, public ISetPoolSystem, public ITickInterface
{
   GENERATED_BODY()
public:
	void Initaialize(UWorld* _world)  override;
	void Execute() override;
	bool ExecuteTick(float deltaSecond) override;
	void SetPool(UPool* pool) override;
private:
	UPROPERTY()
	UWorld* world;
	UPROPERTY()
	UPool* mPool;
};