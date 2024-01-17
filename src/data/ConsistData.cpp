#include "ConsistData.h"
#include "ConsistData.h"
#include <json/json.h>
#include <sstream>
#include <iostream>

CConsistData::CConsistData(const char *jsonString) : DataInfoBase(jsonString)
{
}

void CConsistData::DisplayData()
{
    DisPlayCommonData();

    std::cout << "  数据类型: " << data.DataType << std::endl;
    std::cout << "    信息ID: " << data.content.infoID << std::endl;
    std::cout << "    删除意图: " << data.content.deleteIntention << std::endl;
    std::cout << "    分类分级信息: " << data.content.classifyInformation << std::endl;
    std::cout << "    删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "    删除一致性异常: " << data.content.deleteConsistencyDiff << std::endl;
}

bool CConsistData::TurnStr2Obj(const char *jsonString)
{
    data.DataType = root["data"]["DataType"].asUInt();
    data.content.infoID = root["data"]["content"]["infoID"].asString();
    data.content.deleteIntention = root["data"]["content"]["deleteIntention"].asString();
    data.content.classifyInformation = root["data"]["content"]["classifyInformation"].asString();
    data.content.deleteAlg = root["data"]["content"]["deleteAlg"].asUInt();
    data.content.deleteConsistencyDiff = root["data"]["content"]["deleteConsistencyDiff"].asString();
    return true;
}

void CConsistData::GetInserDataInOrder(std::string &strData) const
{
    // 请根据你的数据库插入语句逻辑完善这个函数
    // 这里只是一个示例，实际上需要根据你的表结构和字段逐个添加
    std::stringstream ss;
    ss << "INSERT INTO YourTable (systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, DataType, infoID, deleteIntention, classifyInformation, deleteAlg, deleteConsistencyDiff, dataHash, datasign, randomidentification) VALUES ("
       << systemID << ", '" << systemIP << "', " << mainCMD << ", " << subCMD << ", '" << evidenceID << "', " << msgVersion << ", '" << submittime << "', " << data.DataType << ", '"
       << data.content.infoID << "', '" << data.content.deleteIntention << "', '" << data.content.classifyInformation << "', " << data.content.deleteAlg << ", '" << data.content.deleteConsistencyDiff
       << "', '" << dataHash << "', '" << datasign << "', '" << randomidentification << "');";

    strData = ss.str();
}
