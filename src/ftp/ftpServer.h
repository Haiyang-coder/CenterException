#pragma
#include <fineftp/server.h>
#include <iostream>
#include "../simpleIni/SimpleIni.h"
#include "../tools/LogPrint.h"

// 总配置文件路径
#define SYSTEM_INI_FILE_PATH "/root/CenterExceptional/config/system_info.ini"
class Cftp
{
public:
    Cftp(/* args */);
    ~Cftp();
    // 按照配置文件初始化ftp服务器
    int StartFtp();

private:
    // 初始化读配置文件的Reader
    int
    InitConfigReader();
    // 将配置文件中的配置读取
    void InitFtpServerInfo();

private:
    // 操作ftp服务器的指针
    fineftp::FtpServer *server;
    // 对外开放的账户
    std::string userName;
    // 对外开放的账户密码
    std::string password;
    // 对外开放的路径
    std::string local_root;
    // 对外开放的端口号
    short port;

private:
    CSimpleIni m_systemIni; // 配置文件读取
};