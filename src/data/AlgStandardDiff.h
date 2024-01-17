#ifndef CALGSTANDARDDIFF_H
#define CALGSTANDARDDIFF_H

#include <string>
#include "DataInfoBase.h"

class CAlgStandardDiff : public DataInfoBase
{
public:
    CAlgStandardDiff(const char *jsonString);

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
        std::string Standard;
        std::string deleteAlgStandardDiff;
    };
    struct Data
    {
        unsigned int DataType;
        Content content;
    };

    // 内部结构体对象
    Data data;
};

#endif // CALGSTANDARDDIFF_H
