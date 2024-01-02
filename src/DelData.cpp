#include "DelData.h"

CDelData::CDelData()
    : systemID(0), systemIP(""), mainCMD(0), subCMD(0), evidenceID(""), msgVersion(0),
      submittime(""), dataHash(""), datasign(""), randomidentification("")
{
    data.DataType = 0;
    data.content.infoID = "";
    data.content.deleteInstruction = "";
    data.content.deletePerformer = "";
    data.content.deletePerformTime = "";
    data.content.timeout = "";
}

void CDelData::DisplayData()
{
    std::cout << "系统ID: " << systemID << std::endl;
    std::cout << "系统IP: " << systemIP << std::endl;
    std::cout << "主命令: " << mainCMD << std::endl;
    std::cout << "子命令: " << subCMD << std::endl;
    std::cout << "证据ID: " << evidenceID << std::endl;
    std::cout << "消息版本: " << msgVersion << std::endl;
    std::cout << "提交时间: " << submittime << std::endl;

    std::cout << "数据:" << std::endl;
    std::cout << "DataType: " << data.DataType << std::endl;
    std::cout << "infoID: " << data.content.infoID << std::endl;
    std::cout << "deleteInstruction: " << data.content.deleteInstruction << std::endl;
    std::cout << "deletePerformer: " << data.content.deletePerformer << std::endl;
    std::cout << "deletePerformTime: " << data.content.deletePerformTime << std::endl;
    std::cout << "timeout: " << data.content.timeout << std::endl;

    std::cout << "数据哈希: " << dataHash << std::endl;
    std::cout << "防伪内容签名: " << datasign << std::endl;
    std::cout << "随机标识: " << randomidentification << std::endl;
}

bool CDelData::TurnStr2Obj(const char *jsonString)
{
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
    if (!ret)
    {
        std::cout << "CDelData 初始化失败了" << std::endl;
        return false;
    }
    assignValues(root);
    return true;
}

void CDelData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"YOUR_DATABASE\".\"YOUR_TABLE\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "DataType, infoID, deleteInstruction, deletePerformer, deletePerformTime, timeout, "
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
       << "'" << data.content.timeout << "', "
       << "'" << dataHash << "', "
       << "'" << datasign << "', "
       << "'" << randomidentification << "');";
    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}

void CDelData::assignValues(const Json::Value &root)
{
    systemID = root.isMember("systemID") ? root["systemID"].asUInt() : 0;
    systemIP = root.isMember("systemIP") ? root["systemIP"].asString() : "";
    mainCMD = root.isMember("mainCMD") ? root["mainCMD"].asUInt() : 0;
    subCMD = root.isMember("subCMD") ? root["subCMD"].asUInt() : 0;
    evidenceID = root.isMember("evidenceID") ? root["evidenceID"].asString() : "";
    msgVersion = root.isMember("msgVersion") ? root["msgVersion"].asUInt() : 0;
    submittime = root.isMember("submittime") ? root["submittime"].asString() : "";

    const Json::Value &jsonData = root["data"];
    dataHash = jsonData.isMember("dataHash") ? jsonData["dataHash"].asString() : "";
    datasign = jsonData.isMember("datasign") ? jsonData["datasign"].asString() : "";
    randomidentification = jsonData.isMember("randomidentification") ? jsonData["randomidentification"].asString() : "";

    data.DataType = jsonData.isMember("DataType") ? jsonData["DataType"].asUInt() : 0;
    const Json::Value &content = jsonData["content"];
    data.content.infoID = content.isMember("infoID") ? content["infoID"].asString() : "";
    data.content.deleteInstruction = content.isMember("deleteInstruction") ? content["deleteInstruction"].asString() : "";
    data.content.deletePerformer = content.isMember("deletePerformer") ? content["deletePerformer"].asString() : "";
    data.content.deletePerformTime = content.isMember("deletePerformTime") ? content["deletePerformTime"].asString() : "";
    data.content.timeout = content.isMember("timeout") ? content["timeout"].asString() : "";
}
