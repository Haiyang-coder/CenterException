#include "DelDupComple.h"
#include <iostream>

DelDupComple::DelDupComple(const char *jsonString) : DataInfoBase(jsonString)
{
    tableName = "delete_DupCompletenessDiff";
}

void DelDupComple::DisplayData()
{
    // 用于显示数据的实现
    DisPlayCommonData();
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  未执行删除的副本集合: " << data.content.deleteDupFailSet << std::endl;
    std::cout << "  已执行删除的副本集合: " << data.content.deleteDupSuccSet << std::endl;
    std::cout << "  副本删除完备性异常: " << data.content.deleteDupCompletenessDiff << std::endl;
}

bool DelDupComple::TurnStr2Obj(const char *jsonString)
{
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteDupFailSet = root["data"]["content"].get("deleteDupFailSet", "").asString();
    data.content.deleteDupSuccSet = root["data"]["content"].get("deleteDupSuccSet", "").asString();
    data.content.deleteDupCompletenessDiff = root["data"]["content"].get("deleteDupCompletenessDiff", "").asString();

    return true;
}

void DelDupComple::GetInserDataInOrder(std::string &strData) const
{
    // 用于生成插入数据库的SQL语句的实现
    strData = "INSERT INTO \"" + modelName + "\".\"" + tableName + "\" (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteDupFailSet, deleteDupSuccSet, deleteDupCompletenessDiff, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + data.content.deleteDupFailSet + "', '" + data.content.deleteDupSuccSet + "', '" + data.content.deleteDupCompletenessDiff + "', '" + dataHash + "', '" + datasign + "', '" + randomidentification + "');";
}
