#include "ECS.h"
#include "DataTableHelper.h"

FDataTableHelper* FDataTableHelper::instance = nullptr;

bool FDataTableHelper::GetDataTableRowData(FName RowId, UDataTable* DataTable, void* TableRow)
{
	if (DataTable !=nullptr && TableRow != nullptr)
	{
		return UDataTableFunctionLibrary::Generic_GetDataTableRowFromName(DataTable, RowId, TableRow);
	}

	UE_LOG(ECS_LOG, Log, TEXT("DataTable is null or TableRow is null !!!"));

	return false;
}