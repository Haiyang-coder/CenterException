#include "UnNotificationCfirm.h"
#include <iostream>

UnNotificationCfirm::UnNotificationCfirm(const char *jsonString) : DataInfoBase(jsonString)
{
}

void UnNotificationCfirm::DisplayData()
{
    // 方法实现用于显示数据
    DisPlayCommonData();
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除通知: " << data.content.deleteNotify << std::endl;
    std::cout << "  删除确认信息: " << data.content.deleteConfirmation << std::endl;
    std::cout << "  删除确认: " << data.content.deleteConfirmationResponse << std::endl;
    std::cout << "  删除通知与确认差异: " << data.content.deleteNotificationConfirmationDiff << std::endl;
}

bool UnNotificationCfirm::TurnStr2Obj(const char *jsonString)
{
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteNotify = root["data"]["content"].get("deleteNotify", "").asString();
    data.content.deleteConfirmation = root["data"]["content"].get("deleteConfirmation", "").asString();
    data.content.deleteConfirmationResponse = root["data"]["content"].get("deleteConfirmationResponse", "").asString();
    data.content.deleteNotificationConfirmationDiff = root["data"]["content"].get("deleteNotificationConfirmationDiff", "").asString();

    return true;
}

void UnNotificationCfirm::GetInserDataInOrder(std::string &strData) const
{
    // 方法实现用于生成插入数据库的SQL语句
    strData = "INSERT INTO YourTableNameHere (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteNotify, deleteConfirmation, deleteConfirmationResponse, deleteNotificationConfirmationDiff, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + data.content.deleteNotify + "', '" + data.content.deleteConfirmation + "', '" + data.content.deleteConfirmationResponse + "', '" + data.content.deleteNotificationConfirmationDiff + "', '" + dataHash + "', '" + datasign + "', '" +
              randomidentification + "');";
}
