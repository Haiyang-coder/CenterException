#include "ftpServer.h"
#include "muduo/base/Logging.h"
#include "tools/Utils.h"

Cftp::Cftp() : m_started(false), server(new fineftp::FtpServer(port))
{
    int ret = InitConfigReader();
    if (ret < 0)
    {
        LOG_INFO << "初始化读配置文件的对象失败";
        exit(ret);
    }
    InitFtpServerInfo();
}

Cftp::~Cftp()
{
    m_started = false;
}

/// @brief 按照配置文件开启ftp服务器
/// @return
int Cftp::StartFtp()
{
    // 初始化ftp服务器
    server->addUserAnonymous(local_root, fineftp::Permission::All);
    // 给了只读权限
    server->addUser(userName, password, local_root, fineftp::Permission::ReadOnly);
    if (server->start(thread_number))
    {
        m_started = true;
        return 0;
    }
    return -1;
}

void Cftp::InitFtpServerInfo()
{
    port = std::stoi(m_systemIni.GetValue("ftp", "PORT", "21"));
    local_root = m_systemIni.GetValue("ftp", "CSV_PATH", "/dm8/saveCsv");
    utils::checkOrCreateDirectory(local_root);
    userName = m_systemIni.GetValue("ftp", "USER_NAME", "ftp_client");
    password = m_systemIni.GetValue("ftp", "PASSWORD", "12345");
    thread_number = std::stoi(m_systemIni.GetValue("ftp", "Thread_number", "4"));
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