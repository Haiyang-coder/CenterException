#ifndef _UNPROCESSED_EXCEPTION_JSON_
#define _UNPROCESSED_EXCEPTION_JSON_
#include "json.h"
#include <iostream>
#include <string>
#include <map>
#include "DataInfoBase.h"

// 确定性删除系统上报数据格式0x4120
class CDelData : public DataInfoBase
{
public:
    CDelData(const char *jsonString);

    // 显示数据的函数
    virtual void DisplayData();

    // 将json字符串转成对象（赋值）
    virtual bool TurnStr2Obj(const char *jsonString);

    // 获取整个类的插入语句
    virtual void GetInserDataInOrder(std::string &strData) const;

public:
    struct Data
    {
        uint16_t DataType;
        struct Content
        {
            std::string infoID;
            std::string deleteInstruction;
            std::string deletePerformer;
            std::string deletePerformTime;
            std::string timeout;
        } content;
    } data;
};

#endif
