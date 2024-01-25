#ifndef CCONSISTDATA_H
#define CCONSISTDATA_H

#include <string>
#include "DataInfoBase.h"

// 删除效果评测系统上报数据格式（删除一致性无效异常数据存证信息0x4224）
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
