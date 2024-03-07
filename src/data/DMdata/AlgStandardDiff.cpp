#include "AlgStandardDiff.h"
#include <iostream>

CAlgStandardDiff::CAlgStandardDiff(const char *jsonString) : DataInfoBase(jsonString)
{
    tableName = "delete_AlgStandardDiff";
}

void CAlgStandardDiff::DisplayData()
{
    // 方法实现用于显示数据
    DisPlayCommonData();
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "  删除算法参数: " << data.content.deleteAlgParam << std::endl;
    std::cout << "  标准: " << data.content.Standard << std::endl;
    std::cout << "  删除方法与标准差异: " << data.content.deleteAlgStandardDiff << std::endl;
}

bool CAlgStandardDiff::TurnStr2Obj(const char *jsonString)
{
    // 将字符串转换成对象
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteAlg = root["data"]["content"].get("deleteAlg", 0).asInt();
    data.content.deleteAlgParam = root["data"]["content"].get("deleteAlgParam", "").asString();
    data.content.Standard = root["data"]["content"].get("Standard", "").asString();
    data.content.deleteAlgStandardDiff = root["data"]["content"].get("deleteAlgStandardDiff", "").asString();
    return true;
}

void CAlgStandardDiff::GetInserDataInOrder(std::string &strData) const
{
    // 方法实现用于生成插入数据库的SQL语句
    strData = "INSERT INTO  \"" + modelName + "\".\"" + tableName + "\" (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteAlg, deleteAlgParam, Standard, deleteAlgStandardDiff, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + std::to_string(data.content.deleteAlg) + "', '" + data.content.deleteAlgParam + "', '" + data.content.Standard + "', '" + data.content.deleteAlgStandardDiff + "', '" + dataHash + "', '" + datasign + "', '" +
              randomidentification + "');";
}
