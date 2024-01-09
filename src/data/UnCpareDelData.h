#ifndef _CUNCPAREDELDATA_H_
#define _CUNCPAREDELDATA_H_

#include <iostream>
#include <string>
#include "json.h"
#include "DataInfoBase.h"

class CUnCpareDelData : public DataInfoBase
{
public:
    CUnCpareDelData(); // 构造函数

    void DisplayData();                                   // 显示数据
    bool TurnStr2Obj(const char *jsonString);             // 将字符串转换成对象
    void GetInserDataInOrder(std::string &strData) const; // 获取插入数据库的SQL语句

private:
    void assignValues(const Json::Value &root); // 分配数值

public:
    struct Data
    {
        uint16_t DataType; // 异常类型代码

        struct Content
        {
            std::string infoID;                      // 信息标识
            std::string deleteIntention;             // 删除意图
            std::string deleteRequest;               // 删除请求
            std::string deleteIntentiongRequestDiff; // 删除请求中删除意图不一致的说明
        } content;
    } data;
};

#endif
