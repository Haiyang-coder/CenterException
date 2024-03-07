#include "Packet.h"
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include "muduo/base/Logging.h"
#include "server/MuduoNet.h"
#define BUFFER_SIZE 2048000

using namespace muduo;
/// @brief 构造数据包
/// @param pData 传入要构造的数据
/// @param nSize 构造成功返回：数据包长度 失败：返回0
CPacket::CPacket(const char *pData, size_t &nSize)
{
	std::cout << "开始构造数据包" << std::endl;
	// TODO目前还有一个缺陷，没有认证和校验，如果校验不通过就丢弃这个包
	size_t i = 0;
	uint16_t temp = 0;
	for (; i < nSize; i++)
	{
		m_sHead = ntohs(*(uint16_t *)(pData + i));
		if (m_sHead == 0x0001)
		{
			i += 2;
			std::cout << "找到了包头" << std::endl;
			std::cout << "现在是第" << i + 1 << "个字节" << std::endl;
			break;
		}
	}
	m_cmdType = ntohs(*(uint16_t *)(pData + i));
	i += 2;
	m_itemType = ntohs(*(uint16_t *)(pData + i));
	i += 2;
	m_msgVersion = ntohs(*(uint16_t *)(pData + i));
	i += 2;
	m_cypherModel = ntohs(*(uint16_t *)(pData + i));
	i++;
	memcpy(&m_veryfyModel, pData + i, 2);
	i++;
	m_saveCmd = ntohl(*(uint32_t *)(pData + i));
	i += 4;
	std::cout << "i = " << i << std::endl;
	for (int j = i; j < i + 4; ++j)
	{
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(pData[j])) << " ";
	}
	m_DataLength = ntohl(*(uint32_t *)(pData + i));
	i += 4;
	// 加上数据部分之后无法构成一个包
	std::cout << "转换字节序之后是：" << std::to_string(m_DataLength) << std::endl;
	if (m_DataLength > nSize)
	{
		std::cout << "构不成一个完成的数据包22222" << std::endl;
		nSize = 0;
		return;
	}
	m_strData.resize(m_DataLength);
	memcpy(const_cast<char *>(m_strData.c_str()), pData + i, m_DataLength);
	i += m_DataLength;
	memcpy(m_veryfyData, pData + i, 16);
	i += 16;
	nSize = i;
	Data();
	return;
}

CPacket::CPacket(std::string str)
{
	m_sHead = (0xEFEF);
	m_cmdType = 0;
	m_itemType = 0;
	m_msgVersion = 0;
	m_cypherModel = 0;
	m_veryfyModel = 0;
	m_saveCmd = 0;
	m_DataLength = str.length();
	m_strData = str;
	memset(m_veryfyData, 0, 16);
}

CPacket::CPacket(const CPacket &packet)
{
	if (&packet == this)
	{
		return;
	}
	m_sHead = packet.m_sHead;
	m_cmdType = packet.m_cmdType;
	m_itemType = packet.m_itemType;
	m_msgVersion = packet.m_msgVersion;
	m_cypherModel = packet.m_cypherModel;
	m_veryfyModel = packet.m_veryfyModel;
	m_saveCmd = packet.m_saveCmd;
	m_DataLength = packet.m_DataLength;
	m_strData = packet.m_strData;
	memcpy(m_veryfyData, packet.m_veryfyData, 16);
}

CPacket::CPacket(CPacket &&other)
	: m_sHead(other.m_sHead),
	  m_cmdType(other.m_cmdType),
	  m_itemType(other.m_itemType),
	  m_msgVersion(other.m_msgVersion),
	  m_cypherModel(other.m_cypherModel),
	  m_veryfyModel(other.m_veryfyModel),
	  m_saveCmd(other.m_saveCmd),
	  m_DataLength(other.m_DataLength),
	  m_strData(std::move(other.m_strData)), // 使用 std::move 移动字符串资源
	  m_strOut(std::move(other.m_strOut)),	 // 使用 std::move 移动字符串资源
	  m_headSize(other.m_headSize)
{
	// 移动构造函数中，通常将其他类型的资源直接移动过来
	// 对于字符串成员，使用 std::move 移动其资源，避免不必要的拷贝
	// 如果有其他类型资源，也可以在这里执行移动操作
	// 同时将原对象的数据设置为默认值，确保不再使用已移动的资源
	// 移动数组资源
	std::memcpy(m_veryfyData, other.m_veryfyData, sizeof(m_veryfyData));
	other.m_veryfyData[16] = {}; // 将数组的数据清零，可以根据实际需求修改
	other.m_headSize = 0;
}

CPacket &CPacket::operator=(const CPacket &packet)
{
	if (this == &packet)
	{
		return *this;
	}
	m_sHead = packet.m_sHead;
	m_cmdType = packet.m_cmdType;
	m_itemType = packet.m_itemType;
	m_msgVersion = packet.m_msgVersion;
	m_cypherModel = packet.m_cypherModel;
	m_veryfyModel = packet.m_veryfyModel;
	m_saveCmd = packet.m_saveCmd;
	m_DataLength = packet.m_DataLength;
	m_strData = packet.m_strData;
	memcpy(m_veryfyData, packet.m_veryfyData, 16);
	return *this;
}

CPacket::~CPacket()
{
}

uint32_t CPacket::Size()
{
	return m_DataLength + 34;
}

const std::string &CPacket::Data()
{
	m_strOut.resize(m_DataLength + 34);
	char *pData = (char *)m_strOut.c_str();
	*(int16_t *)pData = htons(m_sHead);
	pData += 2;
	*(int16_t *)pData = htons(m_cmdType);
	pData += 2;
	*(int16_t *)pData = htons(m_itemType);
	pData += 2;
	*(int16_t *)pData = htons(m_msgVersion);
	pData += 2;
	*(unsigned char *)pData = m_cypherModel;
	pData += 1;
	*(unsigned char *)pData = m_veryfyModel;
	pData += 1;
	*(uint32_t *)pData = htonl(m_saveCmd);
	pData += 4;
	*(uint32_t *)pData = htonl(m_DataLength);
	pData += 4;
	memcpy(pData, m_strData.c_str(), m_strData.size());
	pData += m_strData.size();
	memcpy(pData, m_veryfyData, 16);
	pData += 16;
	return m_strOut;
}

/// @brief 寻找数据包的包头开启
/// @param data [in] 数据流
/// @param nSize [in & out] 数据流长度，返回包头的位置
/// @return 0：找到了 其他：没找到
int CPacket::FindHeadStart(const char *pData, size_t &nSize)
{
	size_t size = nSize;
	bool suc = false;
	for (size_t i = 0; i < size; i++)
	{
		m_sHead = ntohs(*(uint16_t *)(pData + i));
		if (m_sHead == 0xEFEF)
		{
			suc = true;
			LOG_INFO << "找到了包头，现在是第" << i + 1 << "个字节";
			nSize = i;
			return 0;
		}
	}
	if (!suc)
	{
		LOG_INFO << "直到倒数第34个字节也没有找到数据头";
		return -1;
	}
	return -2;
}

/// @brief 检验字符串是否构成一个完整的数据包
/// @param data[in] 字符串
/// @return
int CPacket::ConstructData(const char *pData, size_t &nSize)
{
	// 先判断长度有没有一个包头大，如果包头都不够，直接错误
	size_t i = nSize;
	if (i < m_headSize)
	{
		LOG_INFO << "接收的长度不够组成一个包";
		return -1;
	}
	// 开始寻找包头位置因为后面还有数据所以一直找到倒数34个
	int ret = FindHeadStart(pData, i);
	if (ret != 0)
	{
		LOG_INFO << "没有找到包头开始标志位";
		return -2;
	}
	else
	{
		i += 2;
	}
	m_cmdType = ntohs(*(uint16_t *)(pData + i));
	i += 2;
	m_itemType = ntohs(*(uint16_t *)(pData + i));
	i += 2;
	m_msgVersion = ntohs(*(uint16_t *)(pData + i));
	i += 2;
	m_cypherModel = (unsigned char)*(pData + i);
	i++;
	m_veryfyModel = (unsigned char)*(pData + i);
	i++;
	m_saveCmd = ntohl(*(uint32_t *)(pData + i));
	i += 4;
	m_DataLength = ntohl(*(uint32_t *)(pData + i));
	i += 4;
	// 加上数据部分之后无法构成一个包
	if (m_DataLength > nSize - i - 16)
	{
		LOG_INFO << "数据长度大于整个数据流的长度,构不成一个数据包";
		nSize = i;
		return -3;
	}
	m_strData.resize(m_DataLength);
	memcpy(const_cast<char *>(m_strData.c_str()), pData + i, m_DataLength);
	i += m_DataLength;
	memcpy(m_veryfyData, pData + i, 16);
	i += 16;
	nSize = i;
	return 0;
}
