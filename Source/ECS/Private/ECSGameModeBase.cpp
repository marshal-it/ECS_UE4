// Fill out your copyright notice in the Description page of Project Settings.

#include "ECS.h"
#include "ECSGameModeBase.h"

void AECSGameModeBase::BeginPlay()
{
	////创建系统收集器
	//systems = NewObject<USystemContainer>();
	////创建池并初始化
	//pool = NewObject<UPool>();
	//pool->Init();
	//pool->InitWorldPtr(this->GetWorld());
	////创建系统
	//systems->Add(pool->CreateSystem<UEntityDemoSystem>());
	//systems->Initaialize();
	//systems->Execute();

	Super::BeginPlay();
}

void AECSGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
