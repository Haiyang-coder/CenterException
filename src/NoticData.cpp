#include "NoticData.h"

CNoticData::CNoticData()
    : systemID(0), mainCMD(0), subCMD(0), msgVersion(0)
{
}

void CNoticData::DisplayData()
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
    std::cout << "  sourceDomainID: " << data.content.sourceDomainID << std::endl;
    std::cout << "  nextDomainID: " << data.content.nextDomainID << std::endl;
    std::cout << "  lastDomainID: " << data.content.lastDomainID << std::endl;
    std::cout << "  deleteMethod: " << data.content.deleteMethod << std::endl;
    std::cout << "  deleteDomainSet: " << data.content.deleteDomainSet << std::endl;
    std::cout << "  deleteNotify: " << data.content.deleteNotify << std::endl;
    std::cout << "  deleteNotifyCreateTime: " << data.content.deleteNotifyCreateTime << std::endl;
    std::cout << "  deleteNotifyError: " << data.content.deleteNotifyError << std::endl;

    std::cout << "数据哈希: " << dataHash << std::endl;
    std::cout << "防伪内容签名: " << datasign << std::endl;
    std::cout << "随机标识: " << randomidentification << std::endl;
}

bool CNoticData::TurnStr2Obj(const char *jsonString)
{
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
    if (!ret)
    {
        std::cout << "CNoticData 初始化失败了" << std::endl;
        return false;
    }
    assignValues(root);
    return true;
}

void CNoticData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"YOUR_DATABASE\".\"YOUR_TABLE\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "DataType, userID, infoID, sourceDomainID, nextDomainID, lastDomainID, "
       << "deleteMethod, deleteDomainSet, deleteNotify, deleteNotifyCreateTime, deleteNotifyError, "
       << "dataHash, datasign, randomidentification) VALUES ("
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
       << "'" << data.content.sourceDomainID << "', "
       << "'" << data.content.nextDomainID << "', "
       << "'" << data.content.lastDomainID << "', "
       << "'" << data.content.deleteMethod << "', "
       << "'" << data.content.deleteDomainSet << "', "
       << "'" << data.content.deleteNotify << "', "
       << "'" << data.content.deleteNotifyCreateTime << "', "
       << "'" << data.content.deleteNotifyError << "', "
       << "'" << dataHash << "', "
       << "'" << datasign << "', "
       << "'" << randomidentification << "');";
    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}

void CNoticData::assignValues(const Json::Value &root)
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
    data.content.sourceDomainID = content.isMember("sourceDomainID") ? content["sourceDomainID"].asString() : "";
    data.content.nextDomainID = content.isMember("nextDomainID") ? content["nextDomainID"].asString() : "";
    data.content.lastDomainID = content.isMember("lastDomainId") ? content["lastDomainId"].asString() : "";
    data.content.deleteMethod = content.isMember("deleteMethod") ? content["deleteMethod"].asString() : "";
    data.content.deleteDomainSet = content.isMember("deleteDomainSet") ? content["deleteDomainSet"].asString() : "";
    data.content.deleteNotify = content.isMember("deleteNotify") ? content["deleteNotify"].asString() : "";
    data.content.deleteNotifyCreateTime = content.isMember("deleteNotifyCreateTime") ? content["deleteNotifyCreateTime"].asString() : "";
    data.content.deleteNotifyError = content.isMember("deleteNotifyError") ? content["deleteNotifyError"].asString() : "";

    // 检查并分配字符串类型的值
    dataHash = root.isMember("dataHash") ? root["dataHash"].asString() : "";
    datasign = root.isMember("datasign") ? root["datasign"].asString() : "";
    randomidentification = root.isMember("randomidentification") ? root["randomidentification"].asString() : "";
}
