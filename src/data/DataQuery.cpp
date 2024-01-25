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
    tableVersion = root.get("tableIndex", 0).asInt();
    subtimeStart = root.get("subtimeStart", "").asString();
    subtimeEnd = root.get("subtimeEnd", "").asString();
    return false;
}

void CDataQuery::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"" + modelName + "\".\"" + tableName + "\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "data,"
       << "dataHash, datasign, randomidentification) VALUES ("
       << systemID << ", "
       << "'" << systemIP << "', "
       << mainCMD << ", "
       << subCMD << ", "
       << "'" << evidenceID << "', "
       << msgVersion << ", "
       << "'" << submittime << "', "
       << data << ", "
       << "'" << dataHash << "', "
       << "'" << datasign << "', "
       << "'" << randomidentification << "');";
    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}
