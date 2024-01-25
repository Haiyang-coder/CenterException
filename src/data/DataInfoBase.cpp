#include "DataInfoBase.h"

DataInfoBase::DataInfoBase(const char *jsonStr)
{
  bool ret = InitJsonReader(jsonStr);
  if (!ret)
  {
    LOG(LOG_ERROR, "初始化jsonReader失败了,程序退出");
    exit(-1);
  }
  SetCommonData();
}

DataInfoBase::~DataInfoBase()
{
}

void DataInfoBase::DisPlayCommonData()
{
  std::cout << "系统ID: " << systemID << std::endl;
  std::cout << "系统IP: " << systemIP << std::endl;
  std::cout << "主命令: " << mainCMD << std::endl;
  std::cout << "子命令: " << subCMD << std::endl;
  std::cout << "证据ID: " << evidenceID << std::endl;
  std::cout << "消息版本: " << msgVersion << std::endl;
  std::cout << "提交时间: " << submittime << std::endl;

  std::cout << "数据哈希: " << dataHash << std::endl;
  std::cout << "防伪内容签名: " << datasign << std::endl;
  std::cout << "随机标识: " << randomidentification << std::endl;
}

const std::string &DataInfoBase::GetTableName()
{
  return tableName;
}

void DataInfoBase::SetCommonData()
{
  // 内部方法用于将JSON值分配给对象成员
  systemID = root.get("systemID", 0).asInt();
  systemIP = root.get("systemIP", "NULL").asString();
  mainCMD = root.get("mainCMD", 0).asInt();
  subCMD = root.get("subCMD", 0).asInt();
  evidenceID = root.get("evidenceID", "NULL").asString();
  msgVersion = root.get("msgVersion", 0).asInt();
  submittime = root.get("submittime", "NULL").asString();
  dataHash = root.get("dataHash", "NULL").asString();
  datasign = root.get("datasign", "NULL").asString();
  randomidentification = root.get("randomidentification", "NULL").asString();
  data = root.get("data", "NULL").asString();
}

bool DataInfoBase::InitJsonReader(const char *jsonString)
{
  std::istringstream jsonStream(jsonString);
  bool ret = Json::parseFromStream(reader, jsonStream, &root, nullptr);
  if (!ret)
  {
    std::cout << "jsonReader的 初始化失败了" << std::endl;
    return ret;
  }
  return ret;
}

void DataInfoBase::GetInserDataInOrder(std::string &strData) const
{
  std::stringstream ss;
  ss << "INSERT INTO \"" + modelName + "\".\"" + tableName + "\" ("
     << "systemID, systemIP, mainCMD, subCMD, evidenceID, msgVersion, submittime, "
     << "data,"
     << "dataHash, datasign, randomidentification) VALUES ("
     << systemID << ", "
     << "'" << systemIP << "', "
     << mainCMD << ", "
     << subCMD << ", "
     << "'" << evidenceID << "', "
     << msgVersion << ", "
     << "'" << submittime << "', "
     << data << ", "
     << "'" << dataHash << "', "
     << "'" << datasign << "', "
     << "'" << randomidentification << "');";
  strData = ss.str();
  std::cout << "要执行的sql:" << strData << std::endl;
}