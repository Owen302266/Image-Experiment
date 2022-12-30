

#include "NetClnUdp.h"


CNetClnUdp::CNetClnUdp()
{
	RunSocket =0;
	iplen = 0;
	port = 0;
}

CNetClnUdp::~CNetClnUdp()
{
	if(RunSocket != 0)
	{
		closesocket(RunSocket);
	}
}

int CNetClnUdp::InitNet()
{
	WSADATA wsadata;
	int ret;
	ret = WSAStartup ( 0x101, &wsadata );
	if(ret==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int CNetClnUdp::SendSvr(void * p,int len)
{
	if(RunSocket <=0)
	{
		return 0;
	}
	
	if (sendto(RunSocket,(char *)p,len,0,(struct sockaddr*)&server_sockaddr_in,svlen)==SOCKET_ERROR)
	{	
		return 0;
	}
	else
	{
		return 1;
	}
}


int CNetClnUdp::initCln(char * ip,int Port)
{
	if(InitNet()==0)
	{
		return 0;
	}

	iplen = strlen(ip);
	if(iplen<=0 ||iplen>999 )
	{
		iplen = 0;
		return 0;
	}

	memcpy(svrip,ip,iplen);
	svrip[iplen] = '\0';
	port = Port;
	if(1)
	{///连接
		svlen =sizeof(server_sockaddr_in);
		server_sockaddr_in.sin_family=AF_INET;
		server_sockaddr_in.sin_port=htons(port);                      ///server的监听端口
		server_sockaddr_in.sin_addr.s_addr=inet_addr(ip); ///server的地址 
		RunSocket=socket(AF_INET,SOCK_DGRAM,0);
	}
	return 1;
};