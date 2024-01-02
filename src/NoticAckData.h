#ifndef _CUNEXPECTDELNOTIFYACKDATA_H_
#define _CUNEXPECTDELNOTIFYACKDATA_H_

#include <iostream>
#include <string>
#include <map>
#include "json.h"

class CNotifyAckData
{
public:
    CNotifyAckData(); // 构造函数

    void DisplayData();                                    // 显示数据
    bool TurnStr2Obj(const char *jsonString);              // 将字符串转换成对象
    void GetInsertDataInOrder(std::string &strData) const; // 获取插入数据库的SQL语句

private:
    void assignValues(const Json::Value &root); // 分配数值

public:
    uint32_t systemID;      // 系统ID
    std::string systemIP;   // 系统IP
    uint16_t mainCMD;       // 主命令码
    uint16_t subCMD;        // 子命令码
    std::string evidenceID; // 证据ID
    uint16_t msgVersion;    // 消息版本
    std::string submittime; // 提交时间

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

    std::string dataHash;             // 消息哈希
    std::string datasign;             // 防伪内容签名
    std::string randomidentification; // 随机标识
};

#endif
