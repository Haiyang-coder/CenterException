#include "UnTrigerDelData.h"

CUnTrigerDelData::CUnTrigerDelData(const char *jsonString) : DataInfoBase(jsonString)

{
    tableName = "delete_RequestTriggerDiff";
}

void CUnTrigerDelData::DisplayData()
{
    DisPlayCommonData();

    std::cout << "数据:" << std::endl;
    std::cout << "  信息标识: " << data.infoID << std::endl;
    std::cout << "  删除请求: " << data.deleteRequest << std::endl;
    std::cout << "  删除触发条件: " << data.deleteTriggers << std::endl;
    std::cout << "  删除请求与触发差异: " << data.deleteRequestTriggerDiff << std::endl;
}

bool CUnTrigerDelData::TurnStr2Obj(const char *jsonString)
{
    data.infoID = root["data"]["content"]["infoID"].asString();
    data.deleteRequest = root["data"]["content"]["deleteRequest"].asString();
    data.deleteTriggers = root["data"]["content"]["deleteTriggers"].asString();
    data.deleteRequestTriggerDiff = root["data"]["content"]["deleteRequestTriggerDiff"].asString();
    return false;
}

void CUnTrigerDelData::GetInserDataInOrder(std::string &strData) const
{
    // 拼接插入数据库的SQL语句，具体实现根据数据库类型和表结构进行调整
    strData = "INSERT INTO  \"" + modelName + "\".\"" + tableName + "\" (";
    strData += "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, ";
    strData += "infoID, deleteRequest, deleteTriggers, deleteRequestTriggerDiff, ";
    strData += "dataHash, datasign, randomidentification";
    strData += ") VALUES (";
    strData += std::to_string(systemID) + ", ";
    strData += "'" + systemIP + "', ";
    strData += std::to_string(mainCMD) + ", ";
    strData += std::to_string(subCMD) + ", ";
    strData += "'" + evidenceID + "', ";
    strData += std::to_string(msgVersion) + ", ";
    strData += "'" + submittime + "', ";
    strData += "'" + data.infoID + "', ";
    strData += "'" + data.deleteRequest + "', ";
    strData += "'" + data.deleteTriggers + "', ";
    strData += "'" + data.deleteRequestTriggerDiff + "', ";
    strData += "'" + dataHash + "', ";
    strData += "'" + datasign + "', ";
    strData += "'" + randomidentification + "'";
    strData += ");";
}
