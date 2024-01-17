#include "UnRecoverable.h"
#include <iostream>

UnRecoverable::UnRecoverable(const char *jsonString) : DataInfoBase(jsonString)
{
}

void UnRecoverable::DisplayData()
{
    DisPlayCommonData();
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除副本信息标识: " << data.content.deleteDupInfoID << std::endl;
    std::cout << "  删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "  删除算法参数: " << data.content.deleteAlgParam << std::endl;
    std::cout << "  标准: " << data.content.Standard << std::endl;
    std::cout << "  不可恢复性评估结果差异: " << data.content.deleteAlgStandardIrrecoverableDiff << std::endl;
}

bool UnRecoverable::TurnStr2Obj(const char *jsonString)
{
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteDupInfoID = root["data"]["content"].get("deleteDupInfoID", "").asString();
    data.content.deleteAlg = root["data"]["content"].get("deleteAlg", 0).asInt();
    data.content.deleteAlgParam = root["data"]["content"].get("deleteAlgParam", "").asString();
    data.content.Standard = root["data"]["content"].get("Standard", "").asString();
    data.content.deleteAlgStandardIrrecoverableDiff = root["data"]["content"].get("deleteAlgStandardIrrecoverableDiff", "").asString();

    return true;
}

void UnRecoverable::GetInserDataInOrder(std::string &strData) const
{
    strData = "INSERT INTO YourTableNameHere (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteDupInfoID, deleteAlg, deleteAlgParam, Standard, deleteAlgStandardIrrecoverableDiff, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + data.content.deleteDupInfoID + "', '" + (data.content.deleteAlg) + "', '" + data.content.deleteAlgParam + "', '" + data.content.Standard + "', '" + data.content.deleteAlgStandardIrrecoverableDiff + "', '" +
              dataHash + "', '" + datasign + "', '" + randomidentification + "');";
}
