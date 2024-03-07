#ifndef _CUNEXCEPTDELCOPYDATA_H_
#define _CUNEXCEPTDELCOPYDATA_H_

#include <iostream>
#include <string>
#include "json.h"
#include "DataInfoBase.h"

// 删除异常数据拷贝类
class CDelCopyData : public DataInfoBase
{

public:
    CDelCopyData(const char *jsonString);
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
            std::string globalID;
            std::string deleteInstruction;
            std::string deletePerformer;
            std::string deletePerformTime;
            std::string deleteControlSet;
            std::string deleteDupResult;
        } content;
    } data;
};

#endif
