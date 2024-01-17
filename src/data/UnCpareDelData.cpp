#include "UnCpareDelData.h"

CUnCpareDelData::CUnCpareDelData(const char *jsonString) : DataInfoBase(jsonString)
{
}

void CUnCpareDelData::DisplayData()
{
    DisPlayCommonData();

    std::cout << "数据:" << std::endl;
    std::cout << "  异常类型代码: " << data.DataType << std::endl;
    std::cout << "  信息标识: " << data.content.infoID << std::endl;
    std::cout << "  删除意图: " << data.content.deleteIntention << std::endl;
    std::cout << "  删除请求: " << data.content.deleteRequest << std::endl;
    std::cout << "  删除请求中删除意图不一致的说明: " << data.content.deleteIntentiongRequestDiff << std::endl;
}

bool CUnCpareDelData::TurnStr2Obj(const char *jsonString)
{
    const Json::Value &jsonData = root["data"];
    data.DataType = jsonData.isMember("DataType") ? jsonData["DataType"].asUInt() : 0;
    data.content.infoID = jsonData["content"].isMember("infoID") ? jsonData["content"]["infoID"].asString() : "";
    data.content.deleteIntention = jsonData["content"].isMember("deleteIntention") ? jsonData["content"]["deleteIntention"].asString() : "";
    data.content.deleteRequest = jsonData["content"].isMember("deleteRequest") ? jsonData["content"]["deleteRequest"].asString() : "";
    data.content.deleteIntentiongRequestDiff = jsonData["content"].isMember("deleteIntentiongRequestDiff") ? jsonData["content"]["deleteIntentiongRequestDiff"].asString() : "";

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
