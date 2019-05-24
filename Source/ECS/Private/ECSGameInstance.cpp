// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "EntityDemoSystem.h"
#include "LoadTableSystem.h"
#include "MovementSystem.h"
#include "ECSGameInstance.h"


void UECSGameInstance::Init()
{
	UE_LOG(ECS_LOG, Log, TEXT("UECSGameInstacen Init Finished!!"));
	//创建系统收集器
	systems = NewObject<USystemContainer>();
	//创建池并初始化
	pool = NewObject<UPool>();
	pool->Init();
	pool->InitWorldPtr(this->GetWorld());
	//创建系统
	systems->Add(pool->CreateSystem<ULoadTableSystem>()); //加载表系统
	systems->Add(pool->CreateSystem<UEntityDemoSystem>()); //demo系统
	systems->Add(pool->CreateSystem<UMovementSystem>()); //移动系统
	systems->Initaialize(this->GetWorld()); //系统初始化
	systems->Execute(); //系统执行
}
