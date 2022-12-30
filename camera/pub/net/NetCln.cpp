

#include "NetCln.h"

CNetCln::CNetCln()
{
	RunSocket =0;
}

CNetCln::~CNetCln()
{

}

int CNetCln::InitNet()
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


int CNetCln::SendSvr(void * p,int len)
{
	if(RunSocket <=0)
	{
		return 0;
	}
	return send(RunSocket,(char * )p,len,0);
}


int CNetCln::initCln(char * ip,int Port)
{
	if(InitNet()==0)
	{
		return 0;
	}


	if(1)
	{///Á¬½Ó
		if(Port<=0)
		{
			return 0;
		}
		if(ip==NULL)
		{
			return 0;
		}
		struct sockaddr_in pin;
		struct hostent * h;
		h = gethostbyname(ip);
		if(h==NULL)
		{
			return 0;
		}


		pin.sin_family = AF_INET;
		pin.sin_addr.s_addr = htonl(INADDR_ANY); 
		pin.sin_addr.s_addr = ((struct in_addr *)(h->h_addr))->s_addr;
		pin.sin_port = htons(Port);
	

		RunSocket = socket(AF_INET,SOCK_STREAM,0);
		if(RunSocket<=0)
		{
			return 0;
		}

		if(connect(RunSocket,( struct sockaddr  *)&pin,sizeof(pin))==-1)
		{
			RunSocket= 0;
			return 0;
		}
	}
	return 1;
};