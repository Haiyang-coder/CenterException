#ifndef _CUNEXPECTDELNOTICDATA_H_
#define _CUNEXPECTDELNOTICDATA_H_

#include <iostream>
#include <string>
#include "json.h"
#include "DataInfoBase.h"

// 删除指令通知与确认系统上报数据格式0x4020
class CNoticData : public DataInfoBase
{
public:
    CNoticData(const char *jsonString);
    void DisplayData();
    bool TurnStr2Obj(const char *jsonString);
    void GetInserDataInOrder(std::string &strData) const;

public:
    struct Data
    {
        uint16_t DataType;
        struct Content
        {
            std::string userID;
            std::string infoID;
            std::string sourceDomainID;
            std::string nextDomainID;
            std::string lastDomainID;
            std::string deleteMethod;
            std::string deleteDomainSet;
            std::string deleteNotify;
            std::string deleteNotifyCreateTime;
            std::string deleteNotifyError;
        } content;
    } data;
};

#endif
