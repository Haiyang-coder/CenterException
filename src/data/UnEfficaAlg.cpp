#include "UnEfficaAlg.h"
#include <iostream>

UnEfficaAlg::UnEfficaAlg()
{
}

void UnEfficaAlg::DisplayData()
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
    std::cout << "  删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "  删除算法参数: " << data.content.deleteAlgParam << std::endl;
    std::cout << "  个人信息留存状态: " << data.content.RetentionStatus << std::endl;
    std::cout << "  删除算法与个人信息留存状态差异: " << data.content.deleteAlgRetentionStatusDiff << std::endl;
}

bool UnEfficaAlg::TurnStr2Obj(const char *jsonString)
{
    // 方法实现用于从JSON字符串转换为对象
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    if (!Json::parseFromStream(reader, jsonStream, &root, nullptr))
    {
        std::cout << "UnEfficaAlg 初始化失败了" << std::endl;
        return false;
    }

    // 调用内部方法将JSON值分配给对象成员
    assignValues(root);

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

void UnEfficaAlg::assignValues(const Json::Value &root)
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
    data.content.deleteAlg = root["data"]["content"].get("deleteAlg", 0).asInt();
    data.content.deleteAlgParam = root["data"]["content"].get("deleteAlgParam", "").asString();
    data.content.RetentionStatus = root["data"]["content"].get("RetentionStatus", "").asString();
    data.content.deleteAlgRetentionStatusDiff = root["data"]["content"].get("deleteAlgRetentionStatusDiff", "").asString();
    dataHash = root.get("dataHash", "").asString();
    datasign = root.get("datasign", "").asString();
    randomidentification = root.get("randomidentification", "").asString();
}
