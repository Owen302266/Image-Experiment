
#ifndef AN_NETSVR_UDP_H_ 
#define AN_NETSVR_UDP_H_

#include <winsock.h>
#define socklen_t int
#define RECVBUFLEN  5000

class CNetSvrUdp  
{
public:
	CNetSvrUdp();
	virtual ~CNetSvrUdp();
	unsigned char recvbuf[RECVBUFLEN];
	int recvlen;
	int port;
	bool StartSvr();
	virtual void OnDataRecv()=0;
	int InitNet();
};

#endif 
