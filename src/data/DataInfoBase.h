#pragma once
#include <string>
#include "json.h"

class DataInfoBase
{
private:
    /* data */
public:
    DataInfoBase(/* args */);
    virtual ~DataInfoBase();
    // 展示所有现存的数据
    virtual void DisplayData() = 0;
    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString) = 0;
    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const = 0;

public:
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
};
