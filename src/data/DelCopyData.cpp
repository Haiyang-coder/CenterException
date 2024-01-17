#include "DelCopyData.h"

CDelCopyData::CDelCopyData(const char *jsonString) : DataInfoBase(jsonString)
{
}

void CDelCopyData::DisplayData()
{
    DisPlayCommonData();

    std::cout << "数据:" << std::endl;
    std::cout << "  DataType: " << data.DataType << std::endl;
    std::cout << "  infoID: " << data.content.infoID << std::endl;
    std::cout << "  deleteInstruction: " << data.content.deleteInstruction << std::endl;
    std::cout << "  deletePerformer: " << data.content.deletePerformer << std::endl;
    std::cout << "  deletePerformTime: " << data.content.deletePerformTime << std::endl;
    std::cout << "  deleteControlSet: " << data.content.deleteControlSet << std::endl;
    std::cout << "  deleteDupResult: " << data.content.deleteDupResult << std::endl;
}

bool CDelCopyData::TurnStr2Obj(const char *jsonString)
{
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
