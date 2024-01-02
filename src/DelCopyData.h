#ifndef _CUNEXCEPTDELCOPYDATA_H_
#define _CUNEXCEPTDELCOPYDATA_H_

#include <iostream>
#include <string>
#include "json.h"

// 删除异常数据拷贝类
class CDelCopyData
{

public:
    CDelCopyData();
    // 显示数据的函数
    virtual void DisplayData();
    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString);
    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const;

private:
    // 用于将 Json::Value 中的值赋给 CDelCopyData 对象
    void assignValues(const Json::Value &root);

public:
    uint32_t systemID;
    std::string systemIP;
    uint16_t mainCMD;
    uint16_t subCMD;
    std::string evidenceID;
    uint16_t msgVersion;
    std::string submittime;

    struct Data
    {
        uint16_t DataType;
        struct Content
        {
            std::string infoID;
            std::string deleteInstruction;
            std::string deletePerformer;
            std::string deletePerformTime;
            std::string deleteControlSet;
            std::string deleteDupResult;
        } content;
    } data;

    std::string dataHash;
    std::string datasign;
    std::string randomidentification;
};

#endif
