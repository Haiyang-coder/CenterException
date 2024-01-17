#ifndef _CUNEXPECTDELNOTIFYACKDATA_H_
#define _CUNEXPECTDELNOTIFYACKDATA_H_

#include <iostream>
#include <string>
#include <map>
#include "json.h"
#include "DataInfoBase.h"

class CNotifyAckData : public DataInfoBase
{
public:
    CNotifyAckData(const char *jsonString); // 构造函数

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
            std::string userID;                         // 信息所属主体标识
            std::string infoID;                         // 信息标识
            std::string deleteNotifyAck;                // 删除通知确认
            std::string sourceDomainID;                 // 删除信息的源授权数据处理域ID
            std::map<std::string, std::string> dictAck; // 回传的ACK集合
            std::string deleteNotifyAckError;           // 删除确认通知异常原因
        } content;
    } data;
};

#endif
