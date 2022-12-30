
#ifndef AN_NETSVR_H_ 
#define AN_NETSVR_H_

#include <winsock.h>
#define socklen_t int
#define RECVBUFLEN  1100



class CNetSvr  
{
public:
	CNetSvr();
	virtual ~CNetSvr();
	unsigned char recvbuf[RECVBUFLEN];
	int recvlen;
	int port;

	bool StartSvr();
	int InitNet();
	virtual void OnDataRecv() = 0;
};

#endif 
