#ifndef UNRECOVERABLE_H
#define UNRECOVERABLE_H

#include <string>
#include "DataInfoBase.h"
// 删除效果评测系统上报数据格式（删除副本不可恢复性异常数据存证信息0x4226）
class UnRecoverable : public DataInfoBase
{
public:
    UnRecoverable(const char *jsonString);

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
        std::string deleteDupInfoID;
        std::string deleteAlg;
        std::string deleteAlgParam;
        std::string Standard;
        std::string deleteAlgStandardIrrecoverableDiff;
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

#endif // UNRECOVERABLE_H
