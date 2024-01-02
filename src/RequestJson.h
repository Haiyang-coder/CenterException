#ifndef _REQUESST_JSON_
#define _REQUESST_JSON_
#include "json.h"
#include <iostream>
#include <string>
#include <map>
#include "DataInfoBase.h"

class RequstJsonData : public DataInfoBase
{

public:
    RequstJsonData();
    // 显示数据的函数
    virtual void DisplayData();
    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString);
    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const;

private:
    // 用于将 Json::Value 中的值赋给 JsonData 对象
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
    public:
        std::string objectHash;
        std::string objectSize;
        std::string objectSign;
        std::string title;
        std::string abstract;
        std::string keyWords;
        std::string category;
        std::string others;
        std::string recognizeResult;
    } data;

    std::string dataHash;
    std::string noncesign;
};

#endif