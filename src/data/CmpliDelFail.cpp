#include "CmpliDelFail.h"
#include <iostream>

CmpliDelFail::CmpliDelFail(const char *jsonString) : DataInfoBase(jsonString)
{
    tableName = "delete_EffectEvaDomainSet";
}

void CmpliDelFail::DisplayData()
{
    // 用于显示数据的实现
    DisPlayCommonData();
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除效果评测异常域集合: " << data.content.deleteEffectEvaDomainSet << std::endl;
}

bool CmpliDelFail::TurnStr2Obj(const char *jsonString)
{
    // 从JSON字符串转换为对象的实现
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteEffectEvaDomainSet = root["data"]["content"].get("deleteEffectEvaDomainSet", "").asString();

    return true;
}

void CmpliDelFail::GetInserDataInOrder(std::string &strData) const
{
    // 用于生成插入数据库的SQL语句的实现
    strData = "INSERT INTO  \"" + modelName + "\".\"" + tableName + "\" (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteEffectEvaDomainSet, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + data.content.deleteEffectEvaDomainSet + "', '" + dataHash + "', '" + datasign + "', '" + randomidentification + "');";
}
