/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 University of Connecticut
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Robert Martin <robert.martin@engr.uconn.edu>
 */

#include "aqua-sim-header.h"
#include "aqua-sim-rmac.h"
#include "ns3/header.h"
#include "ns3/buffer.h"
#include "ns3/log.h"
#include "ns3/address-utils.h"

#include <iostream>
#include <bitset>


using namespace ns3;

#include "ns3/address.h"

//Aqua Sim Header

NS_LOG_COMPONENT_DEFINE("AquaSimHeader");
NS_OBJECT_ENSURE_REGISTERED(AquaSimHeader);

AquaSimHeader::AquaSimHeader(void) :
    m_txTime(0), m_direction(NONE), m_nextHop(),
    m_numForwards(0), m_errorFlag(0), m_uId(-1),
    m_pt(-1), m_pr(-1), m_txRange(-1),
    m_freq(-1), m_noise(0), m_status(INVALID), m_timestamp(0)
{
  //m_src.addr(Address());
  //m_dst.addr(Address());
  //m_src.port(0);
  //m_dst.port(0);

  NS_LOG_FUNCTION(this);
}

AquaSimHeader::~AquaSimHeader(void)
{
  NS_LOG_FUNCTION(this);
}

TypeId
AquaSimHeader::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::AquaSimHeader")
    .SetParent<Header>()
    .AddConstructor<AquaSimHeader>()
  ;
  return tid;
}

TypeId
AquaSimHeader::GetInstanceTypeId(void) const
{
  return GetTypeId();
}

uint32_t 
AquaSimHeader::Deserialize(Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_txTime = Seconds ( ( (double) i.ReadU32 ()) / 1000.0 );
  m_direction = i.ReadU8();
  m_numForwards = i.ReadU8();
  m_errorFlag = i.ReadU8();	//wasted space due to only needing 1 bit
  m_uId = i.ReadNtohU16();

  m_pt = i.ReadU32();
  m_pr = i.ReadU32();
  m_txRange = i.ReadU16();
  m_freq = i.ReadU16();
  m_noise = i.ReadU16();
  m_status = i.ReadU8();
  m_timestamp = Seconds ( ( (double) i.ReadU32 ()) / 1000.0 );

  return GetSerializedSize();
}

uint32_t
AquaSimHeader::GetSerializedSize(void) const
{
  /*Currently can be of arbitrary size
  (say 2 bytes for static header fields + 4 bytes for data)
  this can/SHOULD be changed dependent on protocol constrains.
  example can be seen @ main-packet-header.cc*/

  //reserved bytes for header
  return (4 + 1 + 1 + 1 + 2 + 4 + 4 + 2 + 2 + 2 + 1 + 4);
}

void 
AquaSimHeader::Serialize(Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.WriteU32((uint32_t)(m_txTime.GetSeconds() * 1000.0 + 0.5));
  i.WriteU8(m_direction);
  i.WriteU8(m_numForwards);
  i.WriteU8(m_errorFlag);
  i.WriteHtonU16(m_uId);
  //src/dst port + address
  i.WriteU32(m_pt);
  i.WriteU32(m_pr);
  i.WriteU16(m_txRange);
  i.WriteU16(m_freq);
  i.WriteU16(m_noise);
  i.WriteU8(m_status);
  i.WriteU32((uint32_t)(m_timestamp.GetSeconds()*1000.0 + 0.5));
  //m_modName
}

void 
AquaSimHeader::Print(std::ostream &os) const
{
  os << "Packet header is  ";

  os << "TxTime=" << m_txTime << " Direction=";

  if (m_direction == DOWN)
  {
    os << " DOWN";
  }
  if (m_direction == NONE)
  {
    os << " NONE";
  }
  if (m_direction == UP)
  {
    os << " UP";
  }

  os << " Error=";

  if (m_errorFlag == 0)
  {
    os << "False";
  }
  else
  {
    os << "True";
  }

  os << " UniqueID=" << m_uId;

  os << " Tx Power=" << m_pt << " Rx Power=" << m_pr;

  os << " Trans Range=" << m_txRange;

  os << " Frequency=" << m_freq;

  os << " Noise=" << m_noise;

  os << " PacketStatus=";

  if (m_status == RECEPTION)
    {
      os << "RECEPTION";
    }
  if (m_status == COLLISION)
    {
      os << "COLLISION";
    }
  if (m_status == INVALID)
    {
      os << "INVALID";
    }
  os << " Timestamp=" << m_timestamp;

  os << "\n";

}

Time
AquaSimHeader::GetTxTime(void)
{
  return m_txTime;
}

uint32_t
AquaSimHeader::GetSize(void)
{
  return GetSerializedSize();
}

uint8_t
AquaSimHeader::GetDirection(void)
{
  return m_direction;
}

Address
AquaSimHeader::GetNextHop(void)
{
  return m_nextHop;
}

uint8_t
AquaSimHeader::GetNumForwards(void)
{
  return m_numForwards;
}

Address
AquaSimHeader::GetSAddr(void)
{
  return (m_src.addr);
}

Address
AquaSimHeader::GetDAddr(void)
{
  return (m_dst.addr);
}

int32_t 
AquaSimHeader::GetSPort()
{
  return (m_src.port);
}

int32_t 
AquaSimHeader::GetDPort()
{
  return (m_dst.port);
}

bool
AquaSimHeader::GetErrorFlag(void)
{
  return m_errorFlag;
}

uint16_t
AquaSimHeader::GetUId(void)
{
  return m_uId;
}

double
AquaSimHeader::GetTxRange()
{
  return m_txRange;
}
double
AquaSimHeader::GetPt()
{
  return m_pt;
}
double
AquaSimHeader::GetPr()
{
  return m_pr;
}
double
AquaSimHeader::GetFreq()
{
  return m_freq;
}
double
AquaSimHeader::GetNoise()
{
  return m_noise;
}
std::string
AquaSimHeader::GetModName()
{
  return m_modName;
}
uint8_t
AquaSimHeader::GetPacketStatus()
{
  return m_status;
}
Time
AquaSimHeader::GetTimeStamp()
{
  return m_timestamp;
}

void
AquaSimHeader::SetTxTime(Time time)
{
  m_txTime = time;
}

void
AquaSimHeader::SetDirection(uint8_t direction)
{
  m_direction = direction;
}

void
AquaSimHeader::SetNextHop(Address nextHop)
{
  m_nextHop = nextHop;
}

void
AquaSimHeader::SetNumForwards(uint8_t numForwards)
{
  m_numForwards = numForwards;
}

void
AquaSimHeader::SetSAddr(Address sAddr)
{
  m_src.addr = sAddr;
}

void
AquaSimHeader::SetDAddr(Address dAddr)
{
  m_dst.addr = dAddr;
}

void
AquaSimHeader::SetSPort(int32_t sPort)
{
  m_src.port = sPort;
}

void
AquaSimHeader::SetDPort(int32_t dPort)
{
  m_dst.port = dPort;
}

void
AquaSimHeader::SetErrorFlag(bool error)
{
  m_errorFlag = error;
}

void
AquaSimHeader::SetUId(uint16_t uId)
{
  NS_LOG_FUNCTION(this << "this is not unique and must be removed/implemented");
  m_uId = uId;
}

void
AquaSimHeader::SetTxRange(double txRange)
{
  m_txRange = txRange;
}
void
AquaSimHeader::SetPt(double pt)
{
  m_pt = pt;
}
void
AquaSimHeader::SetPr(double pr)
{
  m_pr = pr;
}
void
AquaSimHeader::SetFreq(double freq)
{
  m_freq = freq;
}
void
AquaSimHeader::SetNoise(double noise)
{
  m_noise = noise;
}
void
AquaSimHeader::SetModName(std::string modName)
{
  m_modName = modName;
}

void
AquaSimHeader::SetPacketStatus(uint8_t status)
{
  m_status = status;
}

void
AquaSimHeader::SetTimeStamp(Time timestamp)
{
  m_timestamp = timestamp;
}

bool
AquaSimHeader::CheckConflict()
{
  return (m_txRange > 0 && m_pt < 0) || (m_txRange < 0 && m_pt > 0);
}

void
AquaSimHeader::Stamp(Ptr<Packet> p, double pt, double pr)
{
  m_pt = pt;
  m_pr = pr;
}

//Class RMacHeader

RMacHeader::RMacHeader()
{
  NS_LOG_FUNCTION(this);
}

RMacHeader::~RMacHeader()
{
}

TypeId
RMacHeader::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::RMacHeader")
    .SetParent<Header>()
    .AddConstructor<RMacHeader>()
  ;
  return tid;
}

void
RMacHeader::SetPtype(uint8_t ptype)
{
  m_ptype = ptype;
}
void
RMacHeader::SetPktNum(uint32_t pkNum)
{
  m_pkNum = pkNum;
}
void
RMacHeader::SetDataNum(uint32_t dataNum)
{
  m_dataNum = dataNum;
}
void
RMacHeader::SetBlockNum(uint8_t blockNum)
{
  m_blockNum = blockNum;
}
void
RMacHeader::SetSenderAddr(Address senderAddr)
{
  m_senderAddr = senderAddr;
}
void
RMacHeader::SetRecvAddr(Address recvAddr)
{
  m_recvAddr = recvAddr;
}
void
RMacHeader::SetSt(double st)
{
  m_st = st;
}
void
RMacHeader::SetDuration(double duration)
{
  m_duration = duration;
}
void
RMacHeader::SetInterval(double interval)
{
  m_interval = interval;
}
void
RMacHeader::SetArrivalTime(Time arrivalTime)
{
  m_arrivalTime = arrivalTime;
}
void
RMacHeader::SetTs(double ts)
{
  m_ts = ts;
}


uint8_t
RMacHeader::GetPtype()
{
  return m_ptype;
}
uint32_t
RMacHeader::GetPktNum()
{
  return m_pkNum;
}
uint32_t
RMacHeader::GetDataNum()
{
  return m_dataNum;
}
uint8_t
RMacHeader::GetBlockNum()
{
  return m_blockNum;
}
Address
RMacHeader::GetSenderAddr()
{
  return m_senderAddr;
}
Address
RMacHeader::GetRecvAddr()
{
  return m_recvAddr;
}
double
RMacHeader::GetSt()
{
  return m_st;
}
double
RMacHeader::GetDuration()
{
  return m_duration;
}
double
RMacHeader::GetInterval()
{
  return m_interval;
}
Time
RMacHeader::GetArrivalTime()
{
  return m_arrivalTime;
}
double
RMacHeader::GetTs ()
{
  return m_ts;
}
TypeId
RMacHeader::GetInstanceTypeId(void) const
{
  return GetTypeId();
}

uint32_t
RMacHeader::GetSerializedSize(void) const
{
  return 1+4+4+1+1+1+4+4+4+4+4;
}

void
RMacHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_ptype);
  start.WriteU32 (m_pkNum);
  start.WriteU32 (m_dataNum);
  start.WriteU8 (m_blockNum);
  start.WriteU8 (m_senderAddr.GetLength());
  start.WriteU8 (m_recvAddr.GetLength());
  start.WriteU32 ((uint32_t)m_st * 1000.0 + 0.5);
  start.WriteU32 ((uint32_t)m_duration * 1000.0 + 0.5);
  start.WriteU32 ((uint32_t)m_interval * 1000.0 + 0.5);
  start.WriteU32 ((uint32_t)(m_arrivalTime.GetSeconds() * 1000.0 + 0.5));
  start.WriteU32 ((uint32_t)m_ts * 1000.0 + 0.5);

}

uint32_t
RMacHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_ptype = i.ReadU8();
  m_pkNum = i.ReadU32();
  m_dataNum = i.ReadU32();
  m_blockNum = i.ReadU8();
  ReadFrom(i, m_senderAddr,8);	//read 8bit addr
  ReadFrom(i, m_recvAddr, 8);	//read 8bit addr
  m_st = ( (double) i.ReadU32 ()) / 1000.0;
  m_duration = ( (double) i.ReadU32 ()) / 1000.0;
  m_interval = ( (double) i.ReadU32 ()) / 1000.0;
  m_arrivalTime = Seconds ( ( (double) i.ReadU32 ()) / 1000.0);
  m_ts = ( (double) i.ReadU32 ()) / 1000.0;

  return GetSerializedSize();
}

void
RMacHeader::Print (std::ostream &os) const
{
  os << "RMac Header is ptype=";

  switch(m_ptype)
  {
    case ns3::P_DATA: 	os << "DATA"; break;
    case ns3::P_REV: 	os << "REV"; break;
    case ns3::P_ACKREV: 	os << "ACKREV"; break;
    case ns3::P_ND: 		os << "ND"; break;
    case ns3::P_SACKND: 	os << "SACKND"; break;
    case ns3::P_ACKDATA: 	os << "ACKDATA"; break;
    case ns3::P_SYN: 	os << "SYN"; break;
    default: break;
  }

  os << " PkNum=" << m_pkNum << " DataNum=" << m_dataNum;
  os << " BlockNum=" << m_blockNum << " senderAddr=" << m_senderAddr;
  os << " recvAddr=" << m_recvAddr << " st=" << m_st;
  os << " Duration=" << m_duration << " Interval=" << m_interval;
  os << " ArrivalTime=" << m_arrivalTime << " ts=" << m_ts << "\n";
}


/*
 * AlohaHeader
 */
AlohaHeader::AlohaHeader()
{
}

AlohaHeader::~AlohaHeader()
{
}

TypeId
AlohaHeader::GetTypeId()
{
  static TypeId tid = TypeId("ns3::AlohaHeader")
    .SetParent<Header>()
    .AddConstructor<AlohaHeader>()
  ;
  return tid;
}

int
AlohaHeader::size()
{
  return sizeof(Address)*2 + 1; /*for packet_type*/
}
void
AlohaHeader::SetSA(Address sa)
{
  SA = sa;
}
void
AlohaHeader::SetDA(Address da)
{
  DA = da;
}
Address
AlohaHeader::GetSA()
{
  return SA;
}
Address
AlohaHeader::GetDA()
{
  return DA;
}

uint32_t
AlohaHeader::GetSerializedSize(void) const
{
  return 1+1;
}
void
AlohaHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (SA.GetLength());
  start.WriteU8 (DA.GetLength());
}
uint32_t
AlohaHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  ReadFrom(i, SA,8);	//read 8bit addr
  ReadFrom(i, DA, 8);	//read 8bit addr

  return GetSerializedSize();
}
void
AlohaHeader::Print (std::ostream &os) const
{
  os << "Aloha Header: SendAddress=" << SA << ", DestAddress=" << DA << "\n";
}
TypeId
AlohaHeader::GetInstanceTypeId(void) const
{
  return GetTypeId();
}



/*
 * FamaHeader
 */
FamaHeader::FamaHeader()
{
}

FamaHeader::~FamaHeader()
{
}

TypeId
FamaHeader::GetTypeId()
{
  static TypeId tid = TypeId("ns3::FamaHeader")
    .SetParent<Header>()
    .AddConstructor<FamaHeader>()
  ;
  return tid;
}

int
FamaHeader::size()
{
  return sizeof(Address)*4 + 1; /*for packet_type*/
}
void
FamaHeader::SetSA(Address sa)
{
  SA = sa;
}
void
FamaHeader::SetDA(Address da)
{
  DA = da;
}
Address
FamaHeader::GetSA()
{
  return SA;
}
Address
FamaHeader::GetDA()
{
  return DA;
}

uint32_t
FamaHeader::GetSerializedSize(void) const
{
  return 1+1;
}
void
FamaHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (SA.GetLength());
  start.WriteU8 (DA.GetLength());
}
uint32_t
FamaHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  ReadFrom(i, SA,8);	//read 8bit addr
  ReadFrom(i, DA, 8);	//read 8bit addr

  return GetSerializedSize();
}
void
FamaHeader::Print (std::ostream &os) const
{
  os << "FAMA Header: SendAddress=" << SA << ", DestAddress=" << DA << "\n";
}
TypeId
FamaHeader::GetInstanceTypeId(void) const
{
  return GetTypeId();
}



/*
 * CopeHeader
 */
CopeHeader::CopeHeader()
{
}

CopeHeader::~CopeHeader()
{
}

TypeId
CopeHeader::GetTypeId()
{
  static TypeId tid = TypeId("ns3::CopeHeader")
    .SetParent<Header>()
    .AddConstructor<CopeHeader>()
  ;
  return tid;
}

uint32_t
CopeHeader::size()
{
  return GetSerializedSize(); //return m_size;
}
void
CopeHeader::SetSA(Address sa)
{
  SA = sa;
}
void
CopeHeader::SetDA(Address da)
{
  DA = da;
}
Address
CopeHeader::GetSA()
{
  return SA;
}
Address
CopeHeader::GetDA()
{
  return DA;
}

uint32_t
CopeHeader::GetSerializedSize(void) const
{
  return 1+1;
}
void
CopeHeader::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (SA.GetLength());
  start.WriteU8 (DA.GetLength());
}
uint32_t
CopeHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  ReadFrom(i, SA,8);	//read 8bit addr
  ReadFrom(i, DA, 8);	//read 8bit addr

  return GetSerializedSize();
}
void
CopeHeader::Print (std::ostream &os) const
{
  os << "COPE-MAC Header: packet_type=";
  switch(packet_type) {
    case COPE_ND: os << "COPE_ND"; break;
    case COPE_ND_REPLY: os << "COPE_ND_REPLY"; break;
    case MULTI_REV: os << "MULTI_REV"; break;
    case MULTI_REV_ACK: os << "MULTI_REV_ACK"; break;
    case MULTI_DATA_ACK: os << "MULTI_DATA_ACK"; break;
    default: break;
  }
  os << ", SenderAddress=" << SA << ", DestAddress=" << DA << "\n";
}
TypeId
CopeHeader::GetInstanceTypeId(void) const
{
  return GetTypeId();
}
