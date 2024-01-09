#include "UnTrigerDelData.h"

CUnTrigerDelData::CUnTrigerDelData() : DataInfoBase()

{
}

void CUnTrigerDelData::DisplayData()
{
    std::cout << "系统ID: " << systemID << std::endl;
    std::cout << "系统IP: " << systemIP << std::endl;
    std::cout << "主命令码: " << mainCMD << std::endl;
    std::cout << "子命令码: " << subCMD << std::endl;
    std::cout << "证据ID: " << evidenceID << std::endl;
    std::cout << "消息版本: " << msgVersion << std::endl;
    std::cout << "提交时间: " << submittime << std::endl;

    std::cout << "数据:" << std::endl;
    std::cout << "  信息标识: " << data.infoID << std::endl;
    std::cout << "  删除请求: " << data.deleteRequest << std::endl;
    std::cout << "  删除触发条件: " << data.deleteTriggers << std::endl;
    std::cout << "  删除请求与触发差异: " << data.deleteRequestTriggerDiff << std::endl;

    std::cout << "消息哈希: " << dataHash << std::endl;
    std::cout << "防伪内容签名: " << datasign << std::endl;
    std::cout << "随机标识: " << randomidentification << std::endl;
}

bool CUnTrigerDelData::TurnStr2Obj(const char *jsonString)
{
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(jsonString, root))
    {
        assignValues(root);
        return true;
    }
    return false;
}

void CUnTrigerDelData::GetInserDataInOrder(std::string &strData) const
{
    // 拼接插入数据库的SQL语句，具体实现根据数据库类型和表结构进行调整
    strData = "INSERT INTO  \"YOUR_DATABASE\".\"YOUR_TABLE\" (";
    strData += "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, ";
    strData += "infoID, deleteRequest, deleteTriggers, deleteRequestTriggerDiff, ";
    strData += "dataHash, datasign, randomidentification";
    strData += ") VALUES (";
    strData += std::to_string(systemID) + ", ";
    strData += "'" + systemIP + "', ";
    strData += std::to_string(mainCMD) + ", ";
    strData += std::to_string(subCMD) + ", ";
    strData += "'" + evidenceID + "', ";
    strData += std::to_string(msgVersion) + ", ";
    strData += "'" + submittime + "', ";
    strData += "'" + data.infoID + "', ";
    strData += "'" + data.deleteRequest + "', ";
    strData += "'" + data.deleteTriggers + "', ";
    strData += "'" + data.deleteRequestTriggerDiff + "', ";
    strData += "'" + dataHash + "', ";
    strData += "'" + datasign + "', ";
    strData += "'" + randomidentification + "'";
    strData += ");";
}

void CUnTrigerDelData::assignValues(const Json::Value &root)
{
    // 从JSON对象中读取数据并赋值给类的成员变量
    systemID = root["systemID"].asUInt();
    systemIP = root["systemIP"].asString();
    mainCMD = root["mainCMD"].asUInt();
    subCMD = root["subCMD"].asUInt();
    evidenceID = root["evidenceID"].asString();
    msgVersion = root["msgVersion"].asUInt();
    submittime = root["submittime"].asString();

    data.infoID = root["data"]["content"]["infoID"].asString();
    data.deleteRequest = root["data"]["content"]["deleteRequest"].asString();
    data.deleteTriggers = root["data"]["content"]["deleteTriggers"].asString();
    data.deleteRequestTriggerDiff = root["data"]["content"]["deleteRequestTriggerDiff"].asString();

    dataHash = root["dataHash"].asString();
    datasign = root["datasign"].asString();
    randomidentification = root["randomidentification"].asString();
}
