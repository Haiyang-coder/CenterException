#include "DataQuery.h"
CDataQuery ::CDataQuery(const char *jsonString) : DataInfoBase(jsonString)
{
}

CDataQuery ::~CDataQuery()
{
}

void CDataQuery::DisplayData()
{
}

bool CDataQuery::TurnStr2Obj(const char *jsonString)
{
    getType = root.get("getType", 0).asInt();
    tableIndex = root.get("tableIndex", 0).asInt();
    subtimeStart = root.get("subtimeStart", "").asString();
    subtimeEnd = root.get("subtimeEnd", "").asString();
    return false;
}

void CDataQuery::GetInserDataInOrder(std::string &strData) const
{
}
