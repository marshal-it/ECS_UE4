/**
@ 数据表帮助类
@Author TeenWolf
@Email  731796991@qq.com
@Date   2017-08-29 17:49:13
*/


#pragma once

class ULoadTableSystem;

class ECS_API FDataTableHelper 
{
public:
	static FDataTableHelper* Get()
	{
		if (instance == nullptr)
		{
			instance = new FDataTableHelper();
		}
		return instance;
	}

	TWeakObjectPtr<ULoadTableSystem> GetTableSystem()
	{
		return TableSystemRef;
	}

	void SetLoadTableSystem(TWeakObjectPtr<ULoadTableSystem> TableSystem)
	{
		TableSystemRef = TableSystem;
	}

	bool GetDataTableRowData(FName RowId, UDataTable* DataTable, void* TableRow);

private:
	FDataTableHelper() 
	{
	//	UE_LOG(ECS_LOG, Log, TEXT("DataTableHelper Initalize !!"));
	}
	~FDataTableHelper()
	{
		delete instance;
		instance = nullptr;
	}
	static FDataTableHelper* instance;

	TWeakObjectPtr<ULoadTableSystem> TableSystemRef;
};