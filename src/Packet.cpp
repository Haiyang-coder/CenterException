#include "Packet.h"
#define BUFFER_SIZE 2048000

/// @brief 构造数据包
/// @param pData 传入要构造的数据
/// @param nSize 构造成功返回：数据包长度 失败：返回0
CPacket::CPacket(const char *pData, size_t &nSize)
{
	// TODO目前还有一个缺陷，没有认证和校验，如果校验不通过就丢弃这个包
	size_t i = 0;
	for (; i < nSize; i++)
	{
		if (*(uint16_t *)(pData + i) == 0x0001)
		{
			m_sHead = *(uint16_t *)(pData + i);
			i += 2;
			break;
		}
	}

	// 构不成一个完成的数据包
	if (i + 32 > nSize)
	{
		nSize = 0;
		return;
	}

	m_cmdType = *(uint16_t *)(pData + i);
	i += 2;
	m_itemType = *(uint16_t *)(pData + i);
	i += 2;
	m_msgVersion = *(uint16_t *)(pData + i);
	i += 2;
	m_cypherModel = *(unsigned char *)(pData + i);
	i++;
	m_veryfyModel = *(unsigned char *)(pData + i);
	i++;
	m_saveCmd = *(uint32_t *)(pData + i);
	i += 4;
	m_DataLength = *(uint32_t *)(pData + i);
	i += 4;
	if (m_DataLength + i + 16 > nSize)
	{
		// 加上数据部分之后无法构成一个包
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
	m_sHead = 0xEFEF;
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
	*(int16_t *)pData = m_sHead;
	pData += 2;
	*(int16_t *)pData = m_cmdType;
	pData += 2;
	*(int16_t *)pData = m_itemType;
	pData += 2;
	*(int16_t *)pData = m_msgVersion;
	pData += 2;
	*(unsigned char *)pData = m_cypherModel;
	pData += 1;
	*(unsigned char *)pData = m_veryfyModel;
	pData += 1;
	*(uint32_t *)pData = m_saveCmd;
	pData += 4;
	*(uint32_t *)pData = m_DataLength;
	pData += 4;
	memcpy(pData, m_strData.c_str(), m_strData.size());
	pData += m_strData.size();
	memcpy(pData, m_veryfyData, 16);
	pData += 16;
	return m_strOut;
}
