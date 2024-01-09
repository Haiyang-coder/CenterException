#include "UnRecoverable.h"
#include <iostream>

UnRecoverable::UnRecoverable()
{
}

void UnRecoverable::DisplayData()
{
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
    std::cout << "  删除副本信息标识: " << data.content.deleteDupInfoID << std::endl;
    std::cout << "  删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "  删除算法参数: " << data.content.deleteAlgParam << std::endl;
    std::cout << "  标准: " << data.content.Standard << std::endl;
    std::cout << "  不可恢复性评估结果差异: " << data.content.deleteAlgStandardIrrecoverableDiff << std::endl;
    std::cout << "  消息哈希: " << dataHash << std::endl;
    std::cout << "  防伪内容签名: " << datasign << std::endl;
    std::cout << "  随机标识: " << randomidentification << std::endl;
}

bool UnRecoverable::TurnStr2Obj(const char *jsonString)
{
    // 方法实现用于从JSON字符串转换为对象
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    if (!Json::parseFromStream(reader, jsonStream, &root, nullptr))
    {
        std::cout << "UnRecoverable 初始化失败了" << std::endl;
        return false;
    }

    // 调用内部方法将JSON值分配给对象成员
    assignValues(root);

    return true;
}

void UnRecoverable::GetInserDataInOrder(std::string &strData) const
{
    strData = "INSERT INTO YourTableNameHere (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteDupInfoID, deleteAlg, deleteAlgParam, Standard, deleteAlgStandardIrrecoverableDiff, dataHash, datasign, randomidentification) VALUES ('" +
              std::to_string(systemID) + "', '" + systemIP + "', '" + std::to_string(mainCMD) + "', '" + std::to_string(subCMD) + "', '" + evidenceID + "', '" + std::to_string(msgVersion) + "', '" + submittime + "', '" + std::to_string(data.DataType) + "', '" +
              data.content.infoID + "', '" + data.content.deleteDupInfoID + "', '" + (data.content.deleteAlg) + "', '" + data.content.deleteAlgParam + "', '" + data.content.Standard + "', '" + data.content.deleteAlgStandardIrrecoverableDiff + "', '" +
              dataHash + "', '" + datasign + "', '" + randomidentification + "');";
}

void UnRecoverable::assignValues(const Json::Value &root)
{
    systemID = root.get("systemID", 0).asInt();
    systemIP = root.get("systemIP", "").asString();
    mainCMD = root.get("mainCMD", 0).asInt();
    subCMD = root.get("subCMD", 0).asInt();
    evidenceID = root.get("evidenceID", "").asString();
    msgVersion = root.get("msgVersion", 0).asInt();
    submittime = root.get("submittime", "").asString();
    data.DataType = root["data"].get("DataType", 0).asInt();
    data.content.infoID = root["data"]["content"].get("infoID", "").asString();
    data.content.deleteDupInfoID = root["data"]["content"].get("deleteDupInfoID", "").asString();
    data.content.deleteAlg = root["data"]["content"].get("deleteAlg", 0).asInt();
    data.content.deleteAlgParam = root["data"]["content"].get("deleteAlgParam", "").asString();
    data.content.Standard = root["data"]["content"].get("Standard", "").asString();
    data.content.deleteAlgStandardIrrecoverableDiff = root["data"]["content"].get("deleteAlgStandardIrrecoverableDiff", "").asString();
    dataHash = root.get("dataHash", "").asString();
    datasign = root.get("datasign", "").asString();
    randomidentification = root.get("randomidentification", "").asString();
}
