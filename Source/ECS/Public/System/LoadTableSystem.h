/**
@ 加载数据表
@Author TeenWolf
@Email  731796991@qq.com
@Date   2017-08-23 09:35:59
*/

#pragma once
#include "InitializeSystem.h"
#include "TableTypeId.h"
#include "LoadTableSystem.generated.h"

/////////////////////////////////表定义---Start/////////////////////////////////////////

//总表ID
USTRUCT(BlueprintType)
struct FDataTableId : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FDataTableId()
	:DataTable(nullptr)
	, Desc("all datatable id not contain self!!!"){}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataTable)
	TAssetPtr<UDataTable> DataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataTable)
	FString Desc;
};

//游戏提示配置
USTRUCT(BlueprintType)
struct FGameTipConfigTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FGameTipConfigTable()
		: TipName("Warring")
		, TipContent("Warring your health!!!!") { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString TipName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString TipContent;
};

//实体脚本类
USTRUCT(BlueprintType)
struct FEntityClassAssetTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEntityClassAssetTable()
		: EntityClass(nullptr)
		, EntityClassDec("class asset") { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		TSubclassOf<class AEntity> EntityClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString EntityClassDec;
};

//脚本配置类
USTRUCT(BlueprintType)
struct FEntityClassMakeTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEntityClassMakeTable()
		: EntityClassId("00")
		, EnityComponentId("00")
		, EntityClassDec("class asset") { }

	//脚本ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString EntityClassId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString EnityComponentId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString EntityClassDec;
};

//组件类配置表
USTRUCT(BlueprintType)
struct FEntityComponentTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEntityComponentTable()
		: ComponentClass(nullptr)
		, ComponentClassDec("class asset") { }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		TSubclassOf<class UObject> ComponentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString ComponentClassDec;
};

//实体场景道具表
USTRUCT(BlueprintType)
struct FEntityItemSceneTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEntityItemSceneTable() 
	:ItemName("Item")
	, EntityClassId("00")
	, ItemDesc("Item in scene"){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneItemTable)
		FString EntityClassId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneItemTable)
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneItemTable)
	FTransform ItemTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SceneItemTable)
	FString ItemDesc;
};

//资源表
USTRUCT(BlueprintType)
struct FAssetTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FAssetTable() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		TAssetPtr<UObject> Resource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassAsset)
		FString EntityClassDec;
};

/////////////////////////////////表定义---End/////////////////////////////////////////

class TableBase
{
	virtual void dumy() {}
};

template<typename K, class T>
class TableClass : public TableBase
{
public:
	TMap<K, T*> GetTableMap()
	{
		return TableMap;
	}

	void SetTableMap(K Key, T* Value)
	{
		TableMap.Add(Key, Value);
	}

private:
	TMap<K, T*> TableMap;
};

//加载表系统
UCLASS(Blueprintable, BlueprintType)
class ECS_API ULoadTableSystem : public UObject, public IInitializeSystem
{
   GENERATED_BODY()
public:
	~ULoadTableSystem();
	void Initaialize(UWorld* world)  override;

	//根据ID获得数据
	template<typename T, ETableTypeId E>
	T* GetTableStruct(FString Id)
	{
		if (DataMap.Contains(E))
		{
			TMap<FString, T*> test = static_cast<TableClass<FString, T>*>(DataMap[E])->GetTableMap();
			T* tt = test[Id];
			return tt;
		}
		return nullptr;
	}

	//获得数据序列
	template<typename T, ETableTypeId E>
	TArray<T*> GetTableArray()
	{
		TArray<T*> ArrayData;
		if (DataMap.Contains(E))
		{
			TMap<FString, T*> test = static_cast<TableClass<FString, T>*>(DataMap[E])->GetTableMap();
			test.GenerateValueArray(ArrayData);
		}
		return ArrayData;
	}

	TMap<ETableTypeId, TableBase*> GetDataMap()
	{
		return DataMap;
	}

private:

	template<typename T, ETableTypeId E>
	void InitTableData()
	{
		if (DataTableMap.Contains(E))
		{
			UE_LOG(ECS_LOG, Log, TEXT("Has finded table !!"));
		}
		else
		{
			UE_LOG(ECS_LOG, Log, TEXT("not finded item table !!"));
		}
		TableBase* ComponentClass = new TableClass<FString, T>();
		UDataTable* EntityData = DataTableMap[E];
		TArray<FName> RowNames = EntityData->GetRowNames();
		for (FName Row : RowNames)
		{
			T* Struct = new T();
			if (FDataTableHelper::Get()->GetDataTableRowData(Row, EntityData, Struct))
			{
				static_cast<TableClass<FString, T>*>(ComponentClass)->SetTableMap(Row.ToString(), Struct);
				UE_LOG(ECS_LOG, Log, TEXT(" --%s--Data--Init successful !!"), *Row.ToString());
			}
		}
		DataMap.Add(E, ComponentClass);
	}

	void InitAllDataTable();
private:
	
	TMap<ETableTypeId, TableBase*> DataMap;

	UWorld* _world;

	//总表
	UPROPERTY()
	UDataTable* AllTable;

	//表ID-Data键值表
	UPROPERTY()
	TMap<ETableTypeId, UDataTable*> DataTableMap;
};