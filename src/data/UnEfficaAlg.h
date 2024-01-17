#ifndef UNEFFICAALG_H
#define UNEFFICAALG_H

#include <string>
#include "DataInfoBase.h"

class UnEfficaAlg : public DataInfoBase
{
public:
    UnEfficaAlg(const char *jsonString);

    // 方法用于显示数据
    void DisplayData();

    // 方法用于从JSON字符串转换为对象
    bool TurnStr2Obj(const char *jsonString);

    // 方法用于生成插入数据库的SQL语句
    void GetInserDataInOrder(std::string &strData) const;

private:
    // 内部结构体表示content字段
    struct Content
    {
        std::string infoID;
        int deleteAlg;
        std::string deleteAlgParam;
        std::string RetentionStatus;
        std::string deleteAlgRetentionStatusDiff;
    };

    // 修正：将 DataType 移到 content 中
    struct Data
    {
        unsigned int DataType;
        Content content;
    };

    // 内部结构体对象
    Data data;
};

#endif // UNEFFICAALG_H
