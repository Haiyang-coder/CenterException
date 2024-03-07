
#include "json.h"
#include <fstream>
#include <iostream>
#include <functional>
#include "./dm/sqlGradoperate.h"
#include <ftpServerEx.h>
#include "muduo/net/EventLoop.h"
#include "server/MuduoNet.h"
#include <set>

int main()
{
    // 初始化数据库
    CSqlGradOperate sqlOpera;
    if (sqlOpera.InitDMShemaAndTable() < 0)
    {
        LOG_ERROR << "初始化数据库失败，退出程序";
        exit(-1);
    }
    // 设置数据的备份
    if (sqlOpera.SetDbBackup() < 0)
    {
        LOG_ERROR << "设置数据库备份出错，退出";
        exit(-1);
    }

    //  开启ftp服务
    CftpServerEx ftpServer(&sqlOpera);
    if (ftpServer.StartEx() < 0)
    {
        perror("StartFtp function error!");
        exit(-1);
    }

    // 开启服务器
    muduo::net::EventLoop loop;
    CMuduoServer server(&loop);
    server.Start();
    loop.loop();
    return 0;
}
