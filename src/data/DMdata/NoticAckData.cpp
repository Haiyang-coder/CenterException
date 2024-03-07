#include "NoticAckData.h"

CNotifyAckData::CNotifyAckData(const char *jsonString) : DataInfoBase(jsonString)
{
    tableName = "delete_NotifyAckError";
}

void CNotifyAckData::DisplayData()
{
    DisPlayCommonData();

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
}

bool CNotifyAckData::TurnStr2Obj(const char *jsonString)
{
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
    return true;
}

void CNotifyAckData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"" + modelName + "\".\"" + tableName + "\" ("
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
