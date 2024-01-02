#include "NoticAckData.h"

CNotifyAckData::CNotifyAckData()
    : systemID(0), mainCMD(0), subCMD(0), msgVersion(0)
{
}

void CNotifyAckData::DisplayData()
{
    std::cout << "系统ID: " << systemID << std::endl;
    std::cout << "系统IP: " << systemIP << std::endl;
    std::cout << "主命令: " << mainCMD << std::endl;
    std::cout << "子命令: " << subCMD << std::endl;
    std::cout << "证据ID: " << evidenceID << std::endl;
    std::cout << "消息版本: " << msgVersion << std::endl;
    std::cout << "提交时间: " << submittime << std::endl;

    std::cout << "数据:" << std::endl;
    std::cout << "  DataType: " << data.DataType << std::endl;
    std::cout << "  userID: " << data.content.userID << std::endl;
    std::cout << "  infoID: " << data.content.infoID << std::endl;
    std::cout << "  deleteNotifyAck: " << data.content.deleteNotifyAck << std::endl;
    std::cout << "  sourceDomainID: " << data.content.sourceDomainID << std::endl;

    std::cout << "  dictAck:" << std::endl;
    for (const auto &entry : data.content.dictAck)
    {
        std::cout << "    " << entry.first << ": " << entry.second << std::endl;
    }

    std::cout << "  deleteNotifyAckError: " << data.content.deleteNotifyAckError << std::endl;

    std::cout << "数据哈希: " << dataHash << std::endl;
    std::cout << "防伪内容签名: " << datasign << std::endl;
    std::cout << "随机标识: " << randomidentification << std::endl;
}

bool CNotifyAckData::TurnStr2Obj(const char *jsonString)
{
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
    if (!ret)
    {
        std::cout << "CNotifyAckData 初始化失败了" << std::endl;
        return false;
    }
    assignValues(root);
    return true;
}

void CNotifyAckData::GetInsertDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"YOUR_DATABASE\".\"YOUR_TABLE\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "DataType, userID, infoID, deleteNotifyAck, sourceDomainID, "
       << "dictAck, deleteNotifyAckError, dataHash, datasign, randomidentification) VALUES ("
       << systemID << ", "
       << "'" << systemIP << "', "
       << mainCMD << ", "
       << subCMD << ", "
       << "'" << evidenceID << "', "
       << msgVersion << ", "
       << "'" << submittime << "', "
       << data.DataType << ", "
       << "'" << data.content.userID << "', "
       << "'" << data.content.infoID << "', "
       << "'" << data.content.deleteNotifyAck << "', "
       << "'" << data.content.sourceDomainID << "', "
       << "'"; // Opening single quote for dictAck

    for (const auto &entry : data.content.dictAck)
    {
        ss << entry.first << ":" << entry.second << ",";
    }

    ss.seekp(-1, std::ios_base::end); // Remove trailing comma
    ss << "', "                       // Closing single quote for dictAck
       << "'" << data.content.deleteNotifyAckError << "', "
       << "'" << dataHash << "', "
       << "'" << datasign << "', "
       << "'" << randomidentification << "');";

    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}

void CNotifyAckData::assignValues(const Json::Value &root)
{
    // 检查并分配基本类型的值
    systemID = root.isMember("systemID") ? root["systemID"].asUInt() : 0;
    systemIP = root.isMember("systemIP") ? root["systemIP"].asString() : "";
    mainCMD = root.isMember("mainCMD") ? root["mainCMD"].asUInt() : 0;
    subCMD = root.isMember("subCMD") ? root["subCMD"].asUInt() : 0;
    evidenceID = root.isMember("evidenceID") ? root["evidenceID"].asString() : "";
    msgVersion = root.isMember("msgVersion") ? root["msgVersion"].asUInt() : 0;
    submittime = root.isMember("submittime") ? root["submittime"].asString() : "";

    // 检查并分配 data 对象的值
    const Json::Value &jsonData = root["data"];
    data.DataType = jsonData.isMember("DataType") ? jsonData["DataType"].asUInt() : 0;
    const Json::Value &content = jsonData["content"];
    data.content.userID = content.isMember("userID") ? content["userID"].asString() : "";
    data.content.infoID = content.isMember("infoID") ? content["infoID"].asString() : "";
    data.content.deleteNotifyAck = content.isMember("deleteNotifyAck") ? content["deleteNotifyAck"].asString() : "";
    data.content.sourceDomainID = content.isMember("sourceDomainID") ? content["sourceDomainID"].asString() : "";
    data.content.deleteNotifyAckError = content.isMember("deleteNotifyAckError") ? content["deleteNotifyAckError"].asString() : "";

    // 解析 dictAck
    const Json::Value &dictAck = content["dictAck"];
    if (dictAck.isObject())
    {
        for (const auto &key : dictAck.getMemberNames())
        {
            data.content.dictAck[key] = dictAck[key].asString();
        }
    }

    // 检查并分配字符串类型的值
    dataHash = root.isMember("dataHash") ? root["dataHash"].asString() : "";
    datasign = root.isMember("datasign") ? root["datasign"].asString() : "";
    randomidentification = root.isMember("randomidentification") ? root["randomidentification"].asString() : "";
}
