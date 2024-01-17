#include "RequestJson.h"

RequstJsonData::RequstJsonData() : DataInfoBase("dd")
{
}

void RequstJsonData::DisplayData()
{
    std::cout << "系统ID: " << systemID << std::endl;
    std::cout << "系统IP: " << systemIP << std::endl;
    std::cout << "主命令: " << mainCMD << std::endl;
    std::cout << "子命令: " << subCMD << std::endl;
    std::cout << "证据ID: " << evidenceID << std::endl;
    std::cout << "消息版本: " << msgVersion << std::endl;
    std::cout << "提交时间: " << submittime << std::endl;

    std::cout << "数据:" << std::endl;
    std::cout << "  对象哈希: " << data.objectHash << std::endl;
    std::cout << "  对象大小: " << data.objectSize << std::endl;
    std::cout << "  对象签名: " << data.objectSign << std::endl;
    std::cout << "  标题: " << data.title << std::endl;
    std::cout << "  摘要: " << data.abstract << std::endl;
    std::cout << "  关键词: " << data.keyWords << std::endl;
    std::cout << "  类别: " << data.category << std::endl;
    std::cout << "  其他: " << data.others << std::endl;
    std::cout << "  识别结果: " << data.recognizeResult << std::endl;

    std::cout << "数据哈希: " << dataHash << std::endl;
    std::cout << "随机签名: " << randomidentification << std::endl;
}

/// @brief 将字符串转换成对象
/// @param jsonString
/// @return true成功 false 失败
bool RequstJsonData::TurnStr2Obj(const char *jsonString)
{
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream jsonStream(jsonString);
    bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
    if (!ret)
    {
        std::cout << "RequstJsonData 初始化失败了" << std::endl;
        return false;
    }
    assignValues(root);
    return true;
}

/// @brief 获取将整个类对象插入数据库的sql语句
/// @param strData [in] sql语句
void RequstJsonData::GetInserDataInOrder(std::string &strData) const
{
    std::stringstream ss;
    ss << "INSERT INTO \"TEST_EXCEPTION\".\"TEST_info\" ("
       << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
       << "objectHash, objectSize, objectSign, title, abstract, keyWords, category, others, recognizeResult, "
       << "dataHash, noncesign) VALUES ("
       << systemID << ", "
       << "'" << systemIP << "', "
       << mainCMD << ", "
       << subCMD << ", "
       << "'" << evidenceID << "', "
       << msgVersion << ", "
       << "'" << submittime << "', "
       << "'" << data.objectHash << "', "
       << "'" << data.objectSize << "', "
       << "'" << data.objectSign << "', "
       << "'" << data.title << "', "
       << "'" << data.abstract << "', "
       << "'" << data.keyWords << "', "
       << "'" << data.category << "', "
       << "'" << data.others << "', "
       << "'" << data.recognizeResult << "', "
       << "'" << dataHash << "', "
       << "'" << randomidentification << "');";
    strData = ss.str();
    std::cout << "要执行的sql:" << strData << std::endl;
}

void RequstJsonData::assignValues(const Json::Value &root)
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
    data.objectHash = jsonData.isMember("objectHash") ? jsonData["objectHash"].asString() : "";
    data.objectSize = jsonData.isMember("objectSize") ? jsonData["objectSize"].asString() : "";
    data.objectSign = jsonData.isMember("objectSign") ? jsonData["objectSign"].asString() : "";
    data.title = jsonData.isMember("title") ? jsonData["title"].asString() : "";
    data.abstract = jsonData.isMember("abstract") ? jsonData["abstract"].asString() : "";
    data.keyWords = jsonData.isMember("keyWords") ? jsonData["keyWords"].asString() : "";
    data.category = jsonData.isMember("category") ? jsonData["category"].asString() : "";
    data.others = jsonData.isMember("others") ? jsonData["others"].asString() : "";
    data.recognizeResult = jsonData.isMember("recognizeResult") ? jsonData["recognizeResult"].asString() : "";

    // 检查并分配字符串类型的值
    dataHash = root.isMember("dataHash") ? root["dataHash"].asString() : "";
    randomidentification = root.isMember("noncesign") ? root["noncesign"].asString() : "";
}
