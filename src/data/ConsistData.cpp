#include "ConsistData.h"
#include "ConsistData.h"
#include <json/json.h>
#include <sstream>
#include <iostream>

CConsistData::CConsistData()
{
}

void CConsistData::DisplayData()
{
    std::cout << "数据:" << std::endl;
    std::cout << "  系统ID: " << systemID << std::endl;
    std::cout << "  系统IP: " << systemIP << std::endl;
    std::cout << "  主命令码: " << mainCMD << std::endl;
    std::cout << "  子命令码: " << subCMD << std::endl;
    std::cout << "  证据ID: " << evidenceID << std::endl;
    std::cout << "  消息版本: " << msgVersion << std::endl;
    std::cout << "  提交时间: " << submittime << std::endl;

    std::cout << "  数据类型: " << data.DataType << std::endl;
    std::cout << "  内容:" << std::endl;
    std::cout << "    信息ID: " << data.content.infoID << std::endl;
    std::cout << "    删除意图: " << data.content.deleteIntention << std::endl;
    std::cout << "    分类分级信息: " << data.content.classifyInformation << std::endl;
    std::cout << "    删除算法: " << data.content.deleteAlg << std::endl;
    std::cout << "    删除一致性异常: " << data.content.deleteConsistencyDiff << std::endl;

    std::cout << "  数据哈希: " << dataHash << std::endl;
    std::cout << "  防伪内容签名: " << datasign << std::endl;
    std::cout << "  随机标识: " << randomidentification << std::endl;
}

bool CConsistData::TurnStr2Obj(const char *jsonString)
{
    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(jsonString, root))
    {
        std::cerr << "Failed to parse JSON data" << std::endl;
        return false;
    }

    assignValues(root);
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

void CConsistData::assignValues(const Json::Value &root)
{
    systemID = root["systemID"].asUInt();
    systemIP = root["systemIP"].asString();
    mainCMD = root["mainCMD"].asUInt();
    subCMD = root["subCMD"].asUInt();
    evidenceID = root["evidenceID"].asString();
    msgVersion = root["msgVersion"].asUInt();
    submittime = root["submittime"].asString();

    data.DataType = root["data"]["DataType"].asUInt();
    data.content.infoID = root["data"]["content"]["infoID"].asString();
    data.content.deleteIntention = root["data"]["content"]["deleteIntention"].asString();
    data.content.classifyInformation = root["data"]["content"]["classifyInformation"].asString();
    data.content.deleteAlg = root["data"]["content"]["deleteAlg"].asUInt();
    data.content.deleteConsistencyDiff = root["data"]["content"]["deleteConsistencyDiff"].asString();

    dataHash = root["dataHash"].asString();
    datasign = root["datasign"].asString();
    randomidentification = root["randomidentification"].asString();
}
