#include "ECS.h"
#include "DataTableHelper.h"
#include "ECSGameInstance.h"
#include "LoadTableSystem.h"

ULoadTableSystem::~ULoadTableSystem()
{
	for (TMap<ETableTypeId, TableBase*>::TIterator It(DataMap); It;++It)
	{
		delete It.Value();
	}
	DataMap.Empty();
}

void ULoadTableSystem::Initaialize(UWorld* world)
{
	UE_LOG(ECS_LOG, Log, TEXT("LoadTableSystem Initalize!!"));
	this->_world = world;
	//初始化总表
	InitAllDataTable();
	//初始化组件
	InitTableData<FEntityComponentTable, ETableTypeId::ENTITYCOMPONET_TABLE>();
	//初始化脚本表
	InitTableData<FEntityClassAssetTable, ETableTypeId::ENTITYCLASS_TABLE>();
	//初始化场景道具表
	InitTableData<FEntityItemSceneTable, ETableTypeId::SCENEITEM_TABLE>();
	//初始化资源表
	InitTableData<FAssetTable, ETableTypeId::ASSET_TABLE>();
	////初始化组件
	//InitTableData<FEntityComponentTable, ETableTypeId::ENTITYCOMPONET_TABLE>();
	//初始化组合类
	InitTableData<FEntityClassMakeTable, ETableTypeId::ENTITYMAKE_TABLE>();
	//初始化游戏提示表
	InitTableData<FGameTipConfigTable, ETableTypeId::GAMETIPCONFIG_TABLE>();
	//系统注册
	FDataTableHelper::Get()->SetLoadTableSystem(this);
}

//初始化总表
void ULoadTableSystem::InitAllDataTable()
{
	AllTable = Cast<UECSGameInstance>(UGameplayStatics::GetGameInstance(_world))->TableResources.LoadSynchronous();

	if (AllTable == nullptr)
	{
		UE_LOG(ECS_LOG, Log, TEXT("AllTable data is null !!"));
		return;
	}

	FDataTableId IdData;
	TArray<FName> RowNames;
	RowNames = AllTable->GetRowNames();

	for (FName Row : RowNames)
	{
		if (FDataTableHelper::Get()->GetDataTableRowData(Row, AllTable, &IdData))
		{
			if (IdData.DataTable.LoadSynchronous())
			{
				int32 Type = FCString::Atoi(*Row.ToString());
				DataTableMap.Add(ETableTypeId(Type), IdData.DataTable.LoadSynchronous());
			}
			else
			{
				UE_LOG(ECS_LOG, Log, TEXT("RowId --%s--Data--Init failed !!"), *Row.ToString());
			}
		}
	}
	UE_LOG(ECS_LOG, Log, TEXT("AllTable data init finished !!"));
}