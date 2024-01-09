#include "UnNotificationCfirm.h"
#include <iostream>

UnNotificationCfirm::UnNotificationCfirm() : DataInfoBase()
{
}

void UnNotificationCfirm::DisplayData()
{
    // 方法实现用于显示数据
    std::cout << "数据:" << std::endl;
    std::cout << "  系统ID: " << systemID << std::endl;
    std::cout << "  系统IP: " << systemIP << std::endl;
    std::cout << "  主命令码: " << mainCMD << std::endl;
    std::cout << "  子命令码: " << subCMD << std::endl;
    std::cout << "  证据ID: " << evidenceID << std::endl;
    std::cout << "  消息版本: " << msgVersion << std::endl;
    std::cout << "  提交时间: " << submittime << std::endl;
    std::cout << "  异常类型: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除通知: " << data.content.deleteNotify << std::endl;
    std::cout << "  删除确认信息: " << data.content.deleteConfirmation << std::endl;
    std::cout << "  删除确认: " << data.content.deleteConfirmationResponse << std::endl;
    std::cout << "  删除通知与确认差异: " << data.content.deleteNotificationConfirmationDiff << std::endl;
}

bool UnNotificationCfirm::TurnStr2Obj(const char *jsonString)
{
    // 方法实现用于从JSON字符串转换为对象
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    if (!Json::parseFromStream(reader, jsonStream, &root, nullptr))
    {
        std::cout << "UnNotificationCfirm 初始化失败了" << std::endl;
        return false;
    }

    // 调用内部方法将JSON值分配给对象成员
    assignValues(root);

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

void UnNotificationCfirm::assignValues(const Json::Value &root)
{
    // 内部方法用于将JSON值分配给对象成员
    systemID = root.get("systemID", 0).asInt();
    systemIP = root.get("systemIP", "").asString();
    mainCMD = root.get("mainCMD", 0).asInt();
    subCMD = root.get("subCMD", 0).asInt();
    evidenceID = root.get("evidenceID", "").asString();
    msgVersion = root.get("msgVersion", 0).asInt();
    submittime = root.get("submittime", "").asString();
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteNotify = root["data"]["content"].get("deleteNotify", "").asString();
    data.content.deleteConfirmation = root["data"]["content"].get("deleteConfirmation", "").asString();
    data.content.deleteConfirmationResponse = root["data"]["content"].get("deleteConfirmationResponse", "").asString();
    data.content.deleteNotificationConfirmationDiff = root["data"]["content"].get("deleteNotificationConfirmationDiff", "").asString();
    dataHash = root.get("dataHash", "").asString();
    datasign = root.get("datasign", "").asString();
    randomidentification = root.get("randomidentification", "").asString();
}
