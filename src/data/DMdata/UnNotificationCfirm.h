#ifndef UNNOTIFICATIONCFIRM_H
#define UNNOTIFICATIONCFIRM_H

#include <string>
#include "DataInfoBase.h"

// 删除效果评测系统上报数据格式（删除通知与确认完备性异常数据存证信息0x4223）
class UnNotificationCfirm : public DataInfoBase
{
public:
    UnNotificationCfirm(const char *jsonString);

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
        std::string deleteNotify;
        std::string deleteConfirmation;
        std::string deleteConfirmationResponse;
        std::string deleteNotificationConfirmationDiff;
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

#endif // UNNOTIFICATIONCFIRM_H