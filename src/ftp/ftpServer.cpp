#include "ftpServer.h"

Cftp::Cftp()
{
    int ret = InitConfigReader();
    if (ret < 0)
    {
        PRINT_ERROR(ret, "初始化读配置文件的对象失败");
        exit(ret);
    }
    InitFtpServerInfo();
}

Cftp::~Cftp()
{
    if (server == NULL)
    {
        delete server;
    }
}

/// @brief 按照配置文件开启ftp服务器
/// @return
int Cftp::StartFtp()
{
    // 初始化ftp服务器
    server = new fineftp::FtpServer(port);
    server->addUserAnonymous(local_root, fineftp::Permission::All);
    server->addUser(userName, password, local_root, fineftp::Permission::ReadOnly);
    // server.addUser("Uploader", "12345", local_root, fineftp::Permission::DirList | fineftp::Permission::DirCreate | fineftp::Permission::FileWrite | fineftp::Permission::FileAppend);
    if (server->start(4))
    {
        return 0;
    }
    return -1;
}

void Cftp::InitFtpServerInfo()
{
    port = std::stoi(m_systemIni.GetValue("ftp", "PORT", "21"));
    local_root = m_systemIni.GetValue("ftp", "CSV_PATH", "/dm8");
    userName = m_systemIni.GetValue("ftp", "USER_NAME", "ftp_client");
    password = m_systemIni.GetValue("ftp", "PASSWORD", "12345");
}

/// @brief 初始化读配置文件的对象
/// @return
int Cftp::InitConfigReader()
{
    // 定义ini文档对象
    // 参数为true时保存为UTF-8格式，否则为本地编码格式
    m_systemIni.SetUnicode(true);
    // 是否允许一个关键字对应多个值，默认为允许；若不允许，则将最后一个值作为此关键字关联的值
    m_systemIni.SetMultiKey(false);
    SI_Error ret = 0;
    ret = m_systemIni.LoadFile(SYSTEM_INI_FILE_PATH);
    if (ret < 0)
    {
        std::cout << "加载文件：" << SYSTEM_INI_FILE_PATH << "失败了" << std::endl;
        return ret;
    }
}