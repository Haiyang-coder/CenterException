#include "UnEfficaAlg.h"
#include <iostream>

UnEfficaAlg::UnEfficaAlg(const char *jsonString) : DataInfoBase(jsonString)
{
}

void UnEfficaAlg::DisplayData()
{
    // 方法实现用于显示数据
    DisPlayCommonData();
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "  删除算法参数: " << data.content.deleteAlgParam << std::endl;
    std::cout << "  个人信息留存状态: " << data.content.RetentionStatus << std::endl;
    std::cout << "  删除算法与个人信息留存状态差异: " << data.content.deleteAlgRetentionStatusDiff << std::endl;
}

bool UnEfficaAlg::TurnStr2Obj(const char *jsonString)
{
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteAlg = root["data"]["content"].get("deleteAlg", 0).asInt();
    data.content.deleteAlgParam = root["data"]["content"].get("deleteAlgParam", "").asString();
    data.content.RetentionStatus = root["data"]["content"].get("RetentionStatus", "").asString();
    data.content.deleteAlgRetentionStatusDiff = root["data"]["content"].get("deleteAlgRetentionStatusDiff", "").asString();

    return true;
}

void UnEfficaAlg::GetInserDataInOrder(std::string &strData) const
{
    // 方法实现用于生成插入数据库的SQL语句
    strData = "INSERT INTO YourTableNameHere (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteAlg, deleteAlgParam, RetentionStatus, deleteAlgRetentionStatusDiff, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + std::to_string(data.content.deleteAlg) + "', '" + data.content.deleteAlgParam + "', '" + data.content.RetentionStatus + "', '" + data.content.deleteAlgRetentionStatusDiff + "', '" + dataHash + "', '" + datasign + "', '" +
              randomidentification + "');";
}
