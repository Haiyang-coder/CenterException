#pragma once
#include <string>
#include "json.h"
#include <iostream>
#include "../LogPrint.h"

class DataInfoBase
{
private:
    /* data */
public:
    DataInfoBase(const char *jsonStr);
    virtual ~DataInfoBase();
    // 展示所有现存的数据
    virtual void DisplayData() = 0;
    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString) = 0;
    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const = 0;

protected:
    void DisPlayCommonData();

private:
    // 将json数据中 的公共部分赋值
    void SetCommonData();
    // 初始化json的解析工具
    bool InitJsonReader(const char *jsonString);

public:
    // 下面是所有的通信包通用的数据
    uint32_t systemID;
    std::string systemIP;
    uint16_t mainCMD;
    uint16_t subCMD;
    std::string evidenceID;
    uint16_t msgVersion;
    std::string submittime;
    std::string dataHash;
    std::string datasign;
    std::string randomidentification;

protected:
    // json字符串读取
    Json::CharReaderBuilder reader;
    // 读取字符串后构建的root节点
    Json::Value root;
};
