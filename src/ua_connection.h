#ifndef OPCUA_CONNECTIONHELPER_H_
#define OPCUA_CONNECTIONHELPER_H_
#include "opcua.h"
#include "ua_stackInternalTypes.h"

#include <pthread.h>

enum UA_MessageType
{
	UA_MESSAGETYPE_HEL = 0x48454C, // H E L
	UA_MESSAGETYPE_ACK = 0x41434B, // A C k
	UA_MESSAGETYPE_ERR = 0x455151, // E R R
	UA_MESSAGETYPE_OPN = 0x4F504E, // O P N
	UA_MESSAGETYPE_MSG = 0x4D5347, // M S G
	UA_MESSAGETYPE_CLO = 0x434C4F  // C L O
};
enum connectionState
{
	connectionState_CLOSED,
	connectionState_OPENING,
	connectionState_ESTABLISHED,
	connectionState_CLOSE,
};

typedef struct
{
	UA_UInt32 secureChannelId;
	UA_SymmetricAlgorithmSecurityHeader tokenId;
	UA_DateTime createdAt;
	UA_Int32 revisedLifetime;
}SL_ChannelSecurityToken;

typedef struct
{
	UA_UInt32 protocolVersion;
	UA_UInt32 sendBufferSize;
	UA_UInt32 recvBufferSize;
	UA_UInt32 maxMessageSize;
	UA_UInt32 maxChunkCount;
}TL_buffer;

/* Transport Layer Connection */
typedef struct T_TL_connection
{
	UA_Int32 connectionHandle;
	UA_UInt32 connectionState;
	pthread_t readerThread;
	TL_buffer localConf;
	UA_Int32 (*UA_TL_writer)(struct T_TL_connection* c, UA_ByteString* msg);
	TL_buffer remoteConf;
	UA_String localEndpointUrl;
	UA_String remoteEndpointUrl;
	struct T_SL_Channel* secureChannel;
} UA_TL_connection;


/* Secure Layer Channel */
typedef struct T_SL_Channel
{
	UA_String secureChannelId;
	UA_TL_connection* tlConnection;

	UA_AsymmetricAlgorithmSecurityHeader remoteAsymAlgSettings;
	UA_AsymmetricAlgorithmSecurityHeader localAsymAlgSettings;
	UA_SequenceHeader sequenceHeader;

	UA_UInt32 securityMode;
	UA_ByteString remoteNonce;
	UA_ByteString localNonce;
	UA_UInt32 connectionState;

	SL_ChannelSecurityToken securityToken;
	UA_UInt32 requestId; // request Id of the current request

} UA_SL_Channel;

struct SS_connection
{
	UA_Int32 dummy;
};

//typedef struct T_UA_connection
//{
//	TL_connection transportLayer;
//	struct SL_connection secureLayer;
//	struct SS_connection serviceLayer;
//
//	UA_Boolean newDataToRead;
//	UA_ByteString readData;
//	UA_Boolean newDataToWrite;
//	UA_ByteString writeData;
//} UA_SL_connection;
//


#endif /* OPCUA_CONNECTIONHELPER_H_ */