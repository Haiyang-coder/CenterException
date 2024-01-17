#ifndef CCONSISTDATA_H
#define CCONSISTDATA_H

#include <string>
#include "DataInfoBase.h"

class CConsistData : public DataInfoBase
{
public:
    CConsistData(const char *jsonString);

    void DisplayData();
    bool TurnStr2Obj(const char *jsonString);
    void GetInserDataInOrder(std::string &strData) const;

public:
    struct Content
    {
        std::string infoID;
        std::string deleteIntention;
        std::string classifyInformation;
        uint32_t deleteAlg;
        std::string deleteConsistencyDiff;
    };

    struct Data
    {
        uint32_t DataType;
        Content content;
    };
    Data data;
};

#endif // CCONSISTDATA_H
