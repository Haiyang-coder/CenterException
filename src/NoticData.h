#ifndef _CUNEXPECTDELNOTICDATA_H_
#define _CUNEXPECTDELNOTICDATA_H_

#include <iostream>
#include <string>
#include "json.h"

class CNoticData
{
public:
    CNoticData();
    void DisplayData();
    bool TurnStr2Obj(const char *jsonString);
    void GetInserDataInOrder(std::string &strData) const;

private:
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

    std::string dataHash;
    std::string datasign;
    std::string randomidentification;
};

#endif
