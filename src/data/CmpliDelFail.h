#ifndef CMPLIDELFAIL_H
#define CMPLIDELFAIL_H

#include <string>
#include "DataInfoBase.h"

class CmpliDelFail : public DataInfoBase
{
public:
    CmpliDelFail(const char *jsonString);

    // 用于显示数据
    void DisplayData();

    // 从JSON字符串转换为对象
    bool TurnStr2Obj(const char *jsonString);

    // 用于生成插入数据库的SQL语句
    void GetInserDataInOrder(std::string &strData) const;

private:
    // 内部结构体表示content字段
    struct Content
    {
        std::string infoID;
        std::string deleteEffectEvaDomainSet;
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

#endif // CMPLIDELFAIL_H
