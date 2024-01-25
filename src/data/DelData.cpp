#include "DelData.h"

CDelData::CDelData(const char *jsonString) : DataInfoBase(jsonString)
{
    tableName = "delete_UnOrder";
    data.DataType = 0;
    data.content.infoID = "";
    data.content.deleteInstruction = "";
    data.content.deletePerformer = "";
    data.content.deletePerformTime = "";
    data.content.timeout = "";
}

void CDelData::DisplayData()
{
    DisPlayCommonData();
    std::cout << "DataType: " << data.DataType << std::endl;
    std::cout << "infoID: " << data.content.infoID << std::endl;
    std::cout << "deleteInstruction: " << data.content.deleteInstruction << std::endl;
    std::cout << "deletePerformer: " << data.content.deletePerformer << std::endl;
    std::cout << "deletePerformTime: " << data.content.deletePerformTime << std::endl;
    std::cout << "timeout: " << data.content.timeout << std::endl;
}

bool CDelData::TurnStr2Obj(const char *jsonString)
{
    const Json::Value &jsonData = root["data"];
    data.DataType = jsonData.isMember("DataType") ? jsonData["DataType"].asUInt() : 0;
    const Json::Value &content = jsonData["content"];
    data.content.infoID = content.isMember("infoID") ? content["infoID"].asString() : "";
    data.content.deleteInstruction = content.isMember("deleteInstruction") ? content["deleteInstruction"].asString() : "";
    data.content.deletePerformer = content.isMember("deletePerformer") ? content["deletePerformer"].asString() : "";
    data.content.deletePerformTime = content.isMember("deletePerformTime") ? content["deletePerformTime"].asString() : "";
    data.content.timeout = content.isMember("timeout") ? content["timeout"].asString() : "";
    return true;
}

void CDelData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"" + modelName + "\".\"" + tableName + "\" ("
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
