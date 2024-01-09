#include "CmpliDelFail.h"
#include <iostream>

CmpliDelFail::CmpliDelFail()
{
}

void CmpliDelFail::DisplayData()
{
    // 用于显示数据的实现
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
    std::cout << "  删除效果评测异常域集合: " << data.content.deleteEffectEvaDomainSet << std::endl;
}

bool CmpliDelFail::TurnStr2Obj(const char *jsonString)
{
    // 从JSON字符串转换为对象的实现
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    if (!Json::parseFromStream(reader, jsonStream, &root, nullptr))
    {
        std::cout << "CmpliDelFail 初始化失败了" << std::endl;
        return false;
    }

    // 调用内部方法将JSON值分配给对象成员
    assignValues(root);

    return true;
}

void CmpliDelFail::GetInserDataInOrder(std::string &strData) const
{
    // 用于生成插入数据库的SQL语句的实现
    strData = "INSERT INTO YourTableNameHere (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteEffectEvaDomainSet, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + data.content.deleteEffectEvaDomainSet + "', '" + dataHash + "', '" + datasign + "', '" + randomidentification + "');";
}

void CmpliDelFail::assignValues(const Json::Value &root)
{
    // 将JSON值分配给对象成员的内部实现
    systemID = root.get("systemID", 0).asInt();
    systemIP = root.get("systemIP", "").asString();
    mainCMD = root.get("mainCMD", 0).asInt();
    subCMD = root.get("subCMD", 0).asInt();
    evidenceID = root.get("evidenceID", "").asString();
    msgVersion = root.get("msgVersion", 0).asInt();
    submittime = root.get("submittime", "").asString();
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteEffectEvaDomainSet = root["data"]["content"].get("deleteEffectEvaDomainSet", "").asString();
    dataHash = root.get("dataHash", "").asString();
    datasign = root.get("datasign", "").asString();
    randomidentification = root.get("randomidentification", "").asString();
}
