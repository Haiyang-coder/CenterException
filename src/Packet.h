/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-11-17 11:27:47
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-11-20 10:00:44
 * @FilePath: /ExceptionnalOperaAggregteSystem/src/Packet.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef _PACKET_
#define _PACKET_
#include <cstdint>
#include<string>
#include<string.h>
#pragma pack(push)
#pragma pack(1)

#define CMD_TYPE_DATA 0x00000001
#define CMD_TYPE_DATA 0x00000001
#define DATA_HEAD 0xEFEFEFEF
class CPacket
{
public:
	CPacket(){};
	CPacket(const char*, size_t& nSize);
	CPacket(const CPacket& packet);
	CPacket(std::string);
	CPacket& operator=(const CPacket& packet);
	~CPacket();

	uint32_t Size();
	const std::string& Data();
public:
	uint16_t m_sHead;//版本号,数据头
	uint16_t m_cmdType;//命令类别
	uint16_t m_itemType;//事项类别
	uint16_t m_msgVersion;//消息版本号
	unsigned char m_cypherModel;//加密模式
	unsigned char m_veryfyModel;//认证与校验模式
	uint32_t m_saveCmd;//保留字段
	uint32_t m_DataLength;//数据长度
	std::string m_strData;//数据，json
	unsigned char m_veryfyData[16];//认证和校验域
	std::string m_strOut;	//整個包的數據,将这个数据类序列化，直接发出


private:
	
};
#pragma pack(pop)

#endif