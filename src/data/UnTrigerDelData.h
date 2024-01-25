#ifndef _CUNTRIGERDELDATA_H_
#define _CUNTRIGERDELDATA_H_

#include <iostream>
#include <string>
#include "json.h"
#include "DataInfoBase.h"
// 删除效果评测系统上报数据格式（未按删除触发条件对信息进行删除异常数据存证信息0x4222）
class CUnTrigerDelData : public DataInfoBase
{
public:
    CUnTrigerDelData(const char *jsonString); // 构造函数

    void DisplayData();                                   // 显示数据
    bool TurnStr2Obj(const char *jsonString);             // 将字符串转换成对象
    void GetInserDataInOrder(std::string &strData) const; // 获取插入数据库的SQL语句

public:
    struct DataContent
    {
        std::string infoID;                   // 信息标识
        std::string deleteRequest;            // 删除请求
        std::string deleteTriggers;           // 删除触发条件
        std::string deleteRequestTriggerDiff; // 删除请求与触发差异
    } data;
};

#endif // _CUNTRIGERDELDATA_H_
