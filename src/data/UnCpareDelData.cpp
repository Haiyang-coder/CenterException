#include "UnCpareDelData.h"

CUnCpareDelData::CUnCpareDelData() : DataInfoBase()
{
}

void CUnCpareDelData::DisplayData()
{
    std::cout << "系统ID: " << systemID << std::endl;
    std::cout << "系统IP: " << systemIP << std::endl;
    std::cout << "主命令: " << mainCMD << std::endl;
    std::cout << "子命令: " << subCMD << std::endl;
    std::cout << "证据ID: " << evidenceID << std::endl;
    std::cout << "消息版本: " << msgVersion << std::endl;
    std::cout << "提交时间: " << submittime << std::endl;

    std::cout << "数据:" << std::endl;
    std::cout << "  异常类型代码: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除意图: " << data.content.deleteIntention << std::endl;
    std::cout << "  删除请求: " << data.content.deleteRequest << std::endl;
    std::cout << "  删除请求中删除意图不一致的说明: " << data.content.deleteIntentiongRequestDiff << std::endl;

    std::cout << "数据哈希: " << dataHash << std::endl;
    std::cout << "防伪内容签名: " << datasign << std::endl;
    std::cout << "随机标识: " << randomidentification << std::endl;
}

bool CUnCpareDelData::TurnStr2Obj(const char *jsonString)
{
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
    if (!ret)
    {
        std::cout << "CUnCpareDelData 初始化失败了" << std::endl;
        return false;
    }
    assignValues(root);
    return true;
}

void CUnCpareDelData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"YOUR_DATABASE\".\"YOUR_TABLE\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "DataType, infoID, deleteIntention, deleteRequest, deleteIntentiongRequestDiff, "
       << "dataHash, datasign, randomidentification) VALUES ("
       << systemID << ", "
       << "'" << systemIP << "', "
       << mainCMD << ", "
       << subCMD << ", "
       << "'" << evidenceID << "', "
       << msgVersion << ", "
       << "'" << submittime << "', "
       << data.DataType << ", "
       << "'" << data.content.infoID << "', "
       << "'" << data.content.deleteIntention << "', "
       << "'" << data.content.deleteRequest << "', "
       << "'" << data.content.deleteIntentiongRequestDiff << "', "
       << "'" << dataHash << "', "
       << "'" << datasign << "', "
       << "'" << randomidentification << "');";
    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}

void CUnCpareDelData::assignValues(const Json::Value &root)
{
    systemID = root.isMember("systemID") ? root["systemID"].asUInt() : 0;
    systemIP = root.isMember("systemIP") ? root["systemIP"].asString() : "";
    mainCMD = root.isMember("mainCMD") ? root["mainCMD"].asUInt() : 0;
    subCMD = root.isMember("subCMD") ? root["subCMD"].asUInt() : 0;
    evidenceID = root.isMember("evidenceID") ? root["evidenceID"].asString() : "";
    msgVersion = root.isMember("msgVersion") ? root["msgVersion"].asUInt() : 0;
    submittime = root.isMember("submittime") ? root["submittime"].asString() : "";

    const Json::Value &jsonData = root["data"];
    data.DataType = jsonData.isMember("DataType") ? jsonData["DataType"].asUInt() : 0;
    data.content.infoID = jsonData["content"].isMember("infoID") ? jsonData["content"]["infoID"].asString() : "";
    data.content.deleteIntention = jsonData["content"].isMember("deleteIntention") ? jsonData["content"]["deleteIntention"].asString() : "";
    data.content.deleteRequest = jsonData["content"].isMember("deleteRequest") ? jsonData["content"]["deleteRequest"].asString() : "";
    data.content.deleteIntentiongRequestDiff = jsonData["content"].isMember("deleteIntentiongRequestDiff") ? jsonData["content"]["deleteIntentiongRequestDiff"].asString() : "";

    dataHash = root.isMember("dataHash") ? root["dataHash"].asString() : "";
    datasign = root.isMember("datasign") ? root["datasign"].asString() : "";
    randomidentification = root.isMember("randomidentification") ? root["randomidentification"].asString() : "";
}
