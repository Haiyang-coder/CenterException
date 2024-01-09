#include "DelCopyData.h"

CDelCopyData::CDelCopyData() : DataInfoBase()
{
}

void CDelCopyData::DisplayData()
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
    std::cout << "  infoID: " << data.content.infoID << std::endl;
    std::cout << "  deleteInstruction: " << data.content.deleteInstruction << std::endl;
    std::cout << "  deletePerformer: " << data.content.deletePerformer << std::endl;
    std::cout << "  deletePerformTime: " << data.content.deletePerformTime << std::endl;
    std::cout << "  deleteControlSet: " << data.content.deleteControlSet << std::endl;
    std::cout << "  deleteDupResult: " << data.content.deleteDupResult << std::endl;

    std::cout << "数据哈希: " << dataHash << std::endl;
    std::cout << "防伪内容签名: " << datasign << std::endl;
    std::cout << "随机标识: " << randomidentification << std::endl;
}

bool CDelCopyData::TurnStr2Obj(const char *jsonString)
{
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
    if (!ret)
    {
        std::cout << "CDelCopyData 初始化失败了" << std::endl;
        return false;
    }
    assignValues(root);
    return true;
}

void CDelCopyData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"YOUR_DATABASE\".\"YOUR_TABLE\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "DataType, infoID, deleteInstruction, deletePerformer, deletePerformTime, deleteControlSet, deleteDupResult, "
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
       << "'" << data.content.deleteInstruction << "', "
       << "'" << data.content.deletePerformer << "', "
       << "'" << data.content.deletePerformTime << "', "
       << "'" << data.content.deleteControlSet << "', "
       << "'" << data.content.deleteDupResult << "', "
       << "'" << dataHash << "', "
       << "'" << datasign << "', "
       << "'" << randomidentification << "');";
    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}

void CDelCopyData::assignValues(const Json::Value &root)
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
    data.content.infoID = content.isMember("infoID") ? content["infoID"].asString() : "";
    data.content.deleteInstruction = content.isMember("deleteInstruction") ? content["deleteInstruction"].asString() : "";
    data.content.deletePerformer = content.isMember("deletePerformer") ? content["deletePerformer"].asString() : "";
    data.content.deletePerformTime = content.isMember("deletePerformTime") ? content["deletePerformTime"].asString() : "";
    data.content.deleteControlSet = content.isMember("deleteControlSet") ? content["deleteControlSet"].asString() : "";
    data.content.deleteDupResult = content.isMember("deleteDupResult") ? content["deleteDupResult"].asString() : "";

    // 检查并分配字符串类型的值
    dataHash = root.isMember("dataHash") ? root["dataHash"].asString() : "";
    datasign = root.isMember("datasign") ? root["datasign"].asString() : "";
    randomidentification = root.isMember("randomidentification") ? root["randomidentification"].asString() : "";
}
